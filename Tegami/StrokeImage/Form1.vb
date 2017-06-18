Imports System.Reflection

Public Class Form1
    Dim strokes As List(Of Stroke)
    Dim currentStroke As Stroke
    Dim penWidth As Integer = 2
    Dim penBlack As Pen = New Pen(Color.Black, penWidth)
    Dim penRed As Pen = New Pen(Color.Red, penWidth)
    Dim brushBlack As SolidBrush = New SolidBrush(Color.Black)
    Dim brushRed As SolidBrush = New SolidBrush(Color.Red)
    Dim brushRadius As Integer = 4

    Protected Overrides Function ProcessCmdKey(ByRef message As Message, keys As Keys) As Boolean
        Select Case keys
            Case Keys.Delete
                DeleteCurrentStroke()
                Return True
            Case Keys.Back
                DeleteCurrentStroke()
                Return True
        End Select

        Return MyBase.ProcessCmdKey(message, keys)
    End Function

    Private Sub DeleteCurrentStroke()
        Dim targetIndex As New Integer
        For index As Integer = 0 To strokes.Count - 1
            If currentStroke Is strokes(index) Then
                targetIndex = index - 1
                Exit For
            End If
        Next

        strokes.Remove(currentStroke)

        If strokes.Count = 0 Then
            targetIndex = 0
            Stroke.idCounter = 0
            strokes.Add(New Stroke())
            currentStroke = strokes(targetIndex)
        ElseIf targetIndex < 0 Then
            targetIndex = 0
            currentStroke = strokes(targetIndex)
        Else
            currentStroke = strokes(targetIndex)
        End If

        UpdateTreeView()
        TreeView1.SelectedNode = TreeView1.Nodes(targetIndex)

        Panel1.Invalidate()
    End Sub

    Private Sub Panel1_Paint(sender As Object, e As PaintEventArgs) Handles Panel1.Paint
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

            Dim startPoint As New Point
            For Each point In stroke.points
                Dim rect As New Rectangle(point.X - brushRadius, point.Y - brushRadius, brushRadius * 2, brushRadius * 2)
                e.Graphics.FillEllipse(paintBrush, rect)

                If startPoint = Nothing Then
                    startPoint = point
                Else
                    e.Graphics.DrawLine(paintPen, startPoint, point)
                    startPoint = point
                End If
            Next
        Next
    End Sub

    Private Sub Panel1_Click(sender As Object, e As MouseEventArgs) Handles Panel1.MouseClick
        If currentStroke.points.Count < 2 Then
            currentStroke.points.Add(e.Location)
        Else
            If ModifierKeys.HasFlag(Keys.Control) Then
                currentStroke.points.Add(e.Location)
            Else
                Dim targetIndex As Integer
                For index As Integer = 0 To strokes.Count - 1
                    If currentStroke Is strokes(index) Then
                        targetIndex = index
                        Exit For
                    End If
                Next
                currentStroke = New Stroke()
                currentStroke.points.Add(e.Location)
                strokes.Insert(targetIndex + 1, currentStroke)
            End If
        End If

        UpdateTreeView()
        Panel1.Invalidate()

        For index As Integer = 0 To strokes.Count - 1
            If currentStroke Is strokes(index) Then
                TreeView1.SelectedNode = TreeView1.Nodes(index)
                Exit For
            End If
        Next
    End Sub

    Private Sub UpdateTreeView()
        TreeView1.BeginUpdate()
        TreeView1.Nodes.Clear()
        For index As Integer = 0 To strokes.Count - 1
            TreeView1.Nodes.Add("Stroke " & strokes(index).id)
            For Each point In strokes(index).points
                TreeView1.Nodes(index).Nodes.Add("(" & point.X & "," & point.Y & ")")
            Next
        Next

        TreeView1.EndUpdate()
    End Sub

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
            strokes.Insert(targetNode.Index, copy)
            strokes.Remove(currentStroke)
            currentStroke = copy

            UpdateTreeView()
            TreeView1.SelectedNode = TreeView1.Nodes(targetNode.Index)
        End If
    End Sub

    Private Sub NewToolStripMenuItem3_Click(sender As Object, e As EventArgs) Handles NewToolStripMenuItem3.Click
        If strokes(0).points.Count > 0 Then
            Dim result As DialogResult = MessageBox.Show("Clear all strokes?", "StrokeImage - New", MessageBoxButtons.OKCancel)
            If result = DialogResult.OK Then
                ResetValues()
            End If
        End If
    End Sub

    Private Sub ResetValues()
        Stroke.idCounter = 0
        strokes = New List(Of Stroke) From {New Stroke()}
        currentStroke = strokes(0)
        UpdateTreeView()
        Panel1.Invalidate()
    End Sub

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        ResetValues()

        ' Uses Reflection to set Panel1 to DoubleBuffered
        ' See https://stackoverflow.com/questions/8046560/how-to-stop-flickering-c-sharp-winforms
        Panel1.GetType().InvokeMember("DoubleBuffered", BindingFlags.SetProperty Or BindingFlags.Instance Or BindingFlags.NonPublic, Nothing, Panel1, New Object() {True})
    End Sub

    Private Sub SaveAsToolStripMenuItem3_Click(sender As Object, e As EventArgs) Handles SaveAsToolStripMenuItem3.Click
        Dim result As DialogResult = SaveFileDialog1.ShowDialog()
        If result = DialogResult.OK Then
            Using sw As System.IO.StreamWriter = My.Computer.FileSystem.OpenTextFileWriter(SaveFileDialog1.FileName, False)
                sw.WriteLine("Strokes: " & strokes.Count)
                For Each stroke In strokes
                    sw.WriteLine(stroke.id & ": " & stroke.points.Count)
                    For Each point In stroke.points
                        sw.WriteLine(point.X & " " & point.Y)
                    Next
                Next
            End Using
        End If
    End Sub

    Private Sub DeleteToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles DeleteToolStripMenuItem.Click
        DeleteCurrentStroke()
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
                            strokes.Last().points.Add(New Point(numbers(0), numbers(1)))
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
End Class
