Imports System.Reflection
Imports System.Text
Imports System.Windows

Public Class Form1
    Dim strokes As List(Of Stroke)
    Dim currentStroke As Stroke
    Dim penWidth As Integer = 2
    Dim penBlack As Pen = New Pen(Color.Black, penWidth)
    Dim penRed As Pen = New Pen(Color.Red, penWidth)
    Dim penGrid As Pen = New Pen(Color.LightGray, 1)
    Dim penWhite As Pen = New Pen(Color.Gainsboro, 1)
    Dim brushBlack As SolidBrush = New SolidBrush(Color.Black)
    Dim brushRed As SolidBrush = New SolidBrush(Color.Red)
    Dim bezierBrush As SolidBrush = New SolidBrush(Color.DimGray)
    Dim brushRadius As Integer = 4
    Dim openedFile As String = ""

#Region "Helper Functions"
    Private Sub DeleteCurrentStroke()
        Dim targetIndex As Integer = FindCurrentStrokeIndex() - 1

        strokes.Remove(currentStroke)
        If strokes.Count = 0 Then
            targetIndex = 0
            Stroke.idCounter = 0
            strokes.Add(New Stroke())
        ElseIf targetIndex < 0 Then
            targetIndex = 0
        End If

        currentStroke = strokes(targetIndex)

        UpdateTreeView()
        Panel1.Invalidate()
        TreeView1.SelectedNode = TreeView1.Nodes(targetIndex)
    End Sub

    Private Function FindCurrentStrokeIndex() As Integer
        For index As Integer = 0 To strokes.Count - 1
            If currentStroke Is strokes(index) Then
                Return index
            End If
        Next
    End Function

    Private Function ParseNumbers(line As String) As List(Of Integer)
        Dim numbers As New List(Of Integer)
        Dim currentNumber As New String("")
        For Each character In line
            If Char.IsNumber(character) Then
                currentNumber &= character
            ElseIf currentNumber.Count > 0 Then
                numbers.Add(Integer.Parse(currentNumber))
                currentNumber = ""
            End If
        Next

        If currentNumber.Count > 0 Then
            numbers.Add(Integer.Parse(currentNumber))
        End If

        Return numbers
    End Function

    Private Sub ResetValues()
        Stroke.idCounter = 0
        strokes = New List(Of Stroke) From {New Stroke()}
        currentStroke = strokes(0)
        UpdateTreeView()
        Panel1.Invalidate()
    End Sub

    Private Sub UpdateTreeView()
        TreeView1.BeginUpdate()
        TreeView1.Nodes.Clear()

        For index As Integer = 0 To strokes.Count - 1
            TreeView1.Nodes.Add("Stroke " & strokes(index).id)
            For Each point In strokes(index).bezier.points
                TreeView1.Nodes(index).Nodes.Add("(" & point.X & "," & point.Y & ")")
            Next
        Next

        TreeView1.EndUpdate()
    End Sub
#End Region

#Region "Additional Hotkeys"
    Protected Overrides Function ProcessCmdKey(ByRef message As Message, keys As Keys) As Boolean
        Select Case keys
            Case Keys.Back
                DeleteCurrentStroke()
                Return True
        End Select

        Return MyBase.ProcessCmdKey(message, keys)
    End Function
#End Region

#Region "Form"
    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        ResetValues()

        Label3.Text = TrackBar1.Value
        Label4.Text = TrackBar2.Value

        ' Uses Reflection to set Panel1 to DoubleBuffered
        ' See https://stackoverflow.com/questions/8046560/how-to-stop-flickering-c-sharp-winforms
        Panel1.GetType().InvokeMember("DoubleBuffered", BindingFlags.SetProperty Or BindingFlags.Instance Or BindingFlags.NonPublic, Nothing, Panel1, New Object() {True})
    End Sub
#End Region

#Region "Panel"
    Private Sub Panel1_Click(sender As Object, e As MouseEventArgs) Handles Panel1.MouseClick
        Dim points = currentStroke.bezier.points
        If CheckBox2.Checked Then
            If points.Count < 2 Then
                points.Add(e.Location)
            Else
                If ModifierKeys.HasFlag(Keys.Control) Then
                    points.Add(e.Location)
                Else
                    Dim targetIndex As Integer = FindCurrentStrokeIndex()
                    currentStroke = New Stroke()
                    currentStroke.bezier.points.Add(e.Location)
                    strokes.Insert(targetIndex + 1, currentStroke)
                End If
            End If
        End If

        UpdateTreeView()
        Panel1.Invalidate()
        TreeView1.SelectedNode = TreeView1.Nodes(FindCurrentStrokeIndex())
    End Sub

    Private Sub Panel1_Paint(sender As Object, e As PaintEventArgs) Handles Panel1.Paint
        For i = 50 To 450 Step 50
            e.Graphics.DrawLine(penGrid, New Point(0, i), New Point(500, i))
            e.Graphics.DrawLine(penGrid, New Point(i, 0), New Point(i, 500))
        Next

        Dim paintPen As Pen
        Dim paintBrush As SolidBrush

        For Each stroke In strokes
            If currentStroke Is stroke Then
                paintPen = penRed
                paintBrush = brushRed
            Else
                paintPen = penBlack
                paintBrush = brushBlack
            End If

            ' Draw Bezier
            If CheckBox1.Checked Then
                If stroke.bezier.points.Count > 1 Then
                    Dim size = TrackBar2.Value

                    ' Calculates delta based off of first 0.1 point
                    ' I played around with 
                    Dim measureDelta = 0.1
                    Dim measure = stroke.bezier.FindPosition(measureDelta)
                    Dim distance = measure - stroke.bezier.points(0)
                    Dim lengthSquared = distance.X * distance.X + distance.Y * distance.Y
                    Dim length = Math.Sqrt(lengthSquared)
                    Dim target = TrackBar1.Value
                    Dim delta = target / length * measureDelta

                    For i = 0 To 1 Step delta
                        Dim position = stroke.bezier.FindPosition(i)
                        Dim rect As New Rectangle(position.X - size / 2, position.Y - size / 2, size, size)
                        e.Graphics.FillEllipse(bezierBrush, rect)
                    Next

                    Dim finalPos = stroke.bezier.FindPosition(1)
                    Dim finalRect As New Rectangle(finalPos.X - size / 2, finalPos.Y - size / 2, size, size)
                    e.Graphics.FillEllipse(bezierBrush, finalRect)
                End If
            End If

            ' Draw anchor and lines
            If CheckBox2.Checked Then
                Dim startPoint As New Point
                For Each point In stroke.bezier.points
                    Dim rect As New Rectangle(point.X - brushRadius, point.Y - brushRadius, brushRadius * 2, brushRadius * 2)
                    e.Graphics.FillRectangle(paintBrush, rect)

                    If startPoint = Nothing Then
                        startPoint = point
                    Else
                        e.Graphics.DrawLine(paintPen, startPoint, point)
                        startPoint = point
                    End If
                Next
            End If
        Next
    End Sub
#End Region

#Region "TreeView"
    Private Sub TreeView1_AfterSelect(sender As Object, e As TreeViewEventArgs) Handles TreeView1.AfterSelect
        If e.Node.Level = 0 Then
            currentStroke = strokes(e.Node.Index)
        ElseIf e.Node.Level = 1 Then
            currentStroke = strokes(e.Node.Parent.Index)
        End If

        Panel1.Invalidate()
    End Sub

    Private Sub TreeView1_ItemDrag(sender As Object, e As ItemDragEventArgs) Handles TreeView1.ItemDrag
        If TypeOf (e.Item) Is TreeNode Then
            Dim node As TreeNode = DirectCast(e.Item, TreeNode)

            If node.Level = 1 Then
                node = node.Parent
            End If

            TreeView1.SelectedNode = TreeView1.Nodes(node.Index)
            currentStroke = strokes(node.Index)

            Panel1.Invalidate()
            DoDragDrop(e.Item, DragDropEffects.Move)
        End If
    End Sub

    Private Sub TreeView1_DragEnter(sender As Object, e As DragEventArgs) Handles TreeView1.DragEnter
        e.Effect = e.AllowedEffect
    End Sub

    Private Sub TreeView1_DragDrop(sender As Object, e As DragEventArgs) Handles TreeView1.DragDrop
        Dim mousePosition As Point = TreeView1.PointToClient(New Point(e.X, e.Y))
        Dim targetNode As TreeNode = TreeView1.GetNodeAt(mousePosition)

        If targetNode IsNot Nothing Then
            If targetNode.Level = 1 Then
                targetNode = targetNode.Parent
            End If

            Dim copy As Stroke = currentStroke.Clone()

            Dim index As Integer = FindCurrentStrokeIndex()
            If targetNode.Index > index Then
                strokes.Insert(targetNode.Index + 1, copy)
            Else
                strokes.Insert(targetNode.Index, copy)
            End If

            strokes.Remove(currentStroke)
            currentStroke = copy

            UpdateTreeView()
            TreeView1.SelectedNode = TreeView1.Nodes(targetNode.Index)
        End If
    End Sub
#End Region

#Region "Dialogs"
    Private Sub NewToolStripMenuItem3_Click(sender As Object, e As EventArgs) Handles NewToolStripMenuItem3.Click
        If strokes(0).bezier.points.Count > 0 Then
            Dim result As DialogResult = MessageBox.Show("Clear all strokes?", "StrokeImage - New", MessageBoxButtons.OKCancel)
            If result = DialogResult.OK Then
                ResetValues()
            End If
        End If
    End Sub

    Private Sub OpenToolStripMenuItem2_Click(sender As Object, e As EventArgs) Handles OpenToolStripMenuItem2.Click
        Dim result As DialogResult = OpenFileDialog1.ShowDialog()
        If result = DialogResult.OK Then
            strokes.Clear()
            Dim strokeIDMax As Integer = 0

            Using sr As System.IO.StreamReader = My.Computer.FileSystem.OpenTextFileReader(OpenFileDialog1.FileName)
                Dim numbers As List(Of Integer) = ParseNumbers(sr.ReadLine())
                Dim strokeCount As Integer = numbers(0)

                For index As Integer = 0 To strokeCount - 1
                    numbers = ParseNumbers(sr.ReadLine())
                    Dim strokeID As Integer = numbers(0)
                    Dim pointCount As Integer = numbers(1)

                    If strokeID > strokeIDMax Then
                        strokeIDMax = strokeID
                    End If

                    strokes.Add(New Stroke(strokeID))
                    For pindex As Integer = 0 To pointCount - 1
                        numbers = ParseNumbers(sr.ReadLine())
                        If numbers IsNot Nothing Then
                            strokes.Last().bezier.points.Add(New Point(numbers(0), numbers(1)))
                        End If
                    Next
                Next
            End Using

            Stroke.idCounter = strokeIDMax + 1
            currentStroke = strokes.Last
            UpdateTreeView()
            Panel1.Invalidate()
            TreeView1.SelectedNode = TreeView1.Nodes(TreeView1.Nodes.Count - 1)
        End If
    End Sub

    Private Sub SaveAsToolStripMenuItem3_Click(sender As Object, e As EventArgs) Handles SaveAsToolStripMenuItem3.Click
        Dim result As DialogResult = SaveFileDialog1.ShowDialog()
        If result = DialogResult.OK Then
            Using sw As System.IO.StreamWriter = My.Computer.FileSystem.OpenTextFileWriter(SaveFileDialog1.FileName, False)
                sw.WriteLine("Strokes: " & strokes.Count)
                For Each stroke In strokes
                    sw.WriteLine("Stroke " & stroke.id & ": " & stroke.bezier.points.Count)
                    For Each point In stroke.bezier.points
                        sw.WriteLine(point.X & " " & point.Y)
                    Next
                Next
            End Using
        End If
    End Sub

    Private Sub DeleteToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles DeleteToolStripMenuItem.Click
        DeleteCurrentStroke()
    End Sub

    Private Sub ControlsToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles ControlsToolStripMenuItem.Click
        Dim builder As New StringBuilder
        builder.AppendLine("Click the left panel to create points.")
        builder.AppendLine("Hold down Control to create multi-point strokes.")
        builder.AppendLine("Drag strokes in the right view to change order.")
        builder.AppendLine("Use the arrow keys to navigate between strokes.")

        MessageBox.Show(builder.ToString, "StrokeImage")
    End Sub

    Private Sub TrackBar1_Scroll(sender As Object, e As EventArgs) Handles TrackBar1.Scroll
        Label3.Text = TrackBar1.Value
        Panel1.Invalidate()
    End Sub

    Private Sub TrackBar2_Scroll(sender As Object, e As EventArgs) Handles TrackBar2.Scroll
        Label4.Text = TrackBar2.Value
        Panel1.Invalidate()
    End Sub

    Private Sub CheckBox1_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox1.CheckedChanged
        Panel1.Invalidate()
    End Sub

    Private Sub CheckBox2_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox2.CheckedChanged
        Panel1.Invalidate()
    End Sub
#End Region

#Region "Yolo"
    Private Sub Panel2_Paint(sender As Object, e As PaintEventArgs) Handles Panel2.Paint
        e.Graphics.DrawLine(penWhite, 0, 0, 0, 5)
        e.Graphics.DrawLine(penWhite, 0, 5, 254, 5)
        e.Graphics.DrawLine(penWhite, 253, 0, 253, 5)
    End Sub
#End Region
End Class
