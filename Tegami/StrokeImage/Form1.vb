Imports System.Reflection
Imports System.Text
Imports System.Windows

Public Class Form1
    Dim openedFile As String = ""

    Dim strokes As List(Of Stroke) = New List(Of Stroke) From {New Stroke()}

    Dim currentStroke As Stroke = Nothing
    ' Allows nullable Point
    Dim currentPoint As Point? = Nothing

    Dim movePoints = False
    Dim prevMousePos = New Point()
    Dim prevMoveAmount = New Point()
    Dim currentPointIndex = 0

    Dim penWidth As Integer = 2
    Dim penBlack As Pen = New Pen(Color.Black, penWidth)
    Dim penRed As Pen = New Pen(Color.Red, penWidth)
    Dim penGrid As Pen = New Pen(Color.LightGray, 1)
    Dim penWhite As Pen = New Pen(Color.Gainsboro, 1)

    Dim brushBlack As SolidBrush = New SolidBrush(Color.Black)
    Dim brushRed As SolidBrush = New SolidBrush(Color.Red)
    Dim bezierBrush As SolidBrush = New SolidBrush(Color.DimGray)
    Dim brushRadius As Integer = 4

    Dim pointCollision = 8.0
    Dim lineCollision = 8.0

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
    End Sub

    Private Sub DeleteCurrentPoint()
        Dim pointIndex As Integer = FindCurrentPointIndex() - 1

        Dim points = currentStroke.bezier.points
        points.Remove(currentPoint)
        If points.Count = 0 Then
            DeleteCurrentStroke()
        End If

        Deselect()
    End Sub

    Private Function FindCurrentStrokeIndex() As Integer
        For index As Integer = 0 To strokes.Count - 1
            If currentStroke Is strokes(index) Then
                Return index
            End If
        Next
    End Function

    Private Function FindCurrentPointIndex() As Integer
        For index As Integer = 0 To currentStroke.bezier.points.Count - 1
            If currentPoint = currentStroke.bezier.points(index) Then
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
        currentPoint = Nothing
        RadioButton1.Checked = True
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

        TreeView1.Focus()

        TreeView1.EndUpdate()
    End Sub

    Private Sub UpdateAll()
        UpdateTreeView()
        Panel1.Invalidate()
        If Not currentPoint Is Nothing Then
            TreeView1.SelectedNode = TreeView1.Nodes(FindCurrentStrokeIndex()).Nodes(FindCurrentPointIndex)
        ElseIf Not currentStroke Is Nothing Then
            TreeView1.SelectedNode = TreeView1.Nodes(FindCurrentStrokeIndex())
        End If
    End Sub

    Private Function FindDistance(p1 As Point, p2 As Point) As Double
        Dim distance = p2 - p1
        Dim lengthSquared = distance.X * distance.X + distance.Y * distance.Y
        Dim length = Math.Sqrt(lengthSquared)
        Return length
    End Function

    ' http//www.vcskicks.com/code-snippet/point-projection.php
    Private Function Project(lineStart As Point, lineEnd As Point, toProject As Point) As Point
        Dim m As Double = (lineEnd.Y - lineStart.Y) / (lineEnd.X - lineStart.X)
        Dim b As Double = lineStart.Y - (m * lineStart.X)

        Dim x As Double = (m * toProject.Y + toProject.X - m * b) / (m * m + 1)
        Dim y As Double = (m * m * toProject.Y + m * toProject.X + b) / (m * m + 1)

        Return New Point(x, y)
    End Function

    Private Function WithinBounds(mouse As Point, p1 As Point, p2 As Point)
        Dim left = 0.0
        Dim right = 0.0
        Dim top = 0.0
        Dim bot = 0.0

        If p1.X < p2.X Then
            left = p1.X
            right = p2.X
        Else
            left = p2.X
            right = p1.X
        End If

        If p1.Y < p2.Y Then
            top = p1.Y
            bot = p2.Y
        Else
            top = p2.Y
            bot = p1.Y
        End If

        If mouse.X < left Or mouse.X > right Or mouse.Y < top Or mouse.Y > bot Then
            Return False
        Else
            Return True
        End If
    End Function

    Private Sub MakeSelection(mouse As Point)
        movePoints = True
        prevMousePos = mouse
        prevMoveAmount = New Point()

        ' Checks for point collision
        For i = strokes.Count - 1 To 0 Step -1
            Dim points = strokes(i).bezier.points
            For j = points.Count - 1 To 0 Step -1
                Dim point = points(j)
                Dim length = FindDistance(mouse, point)
                If length < pointCollision Then
                    currentStroke = strokes(i)
                    currentPoint = point
                    currentPointIndex = j
                    Return
                End If
            Next
        Next

        ' Checks for line collision
        For i = strokes.Count - 1 To 0 Step -1
            Dim points = strokes(i).bezier.points
            If points.Count > 1 Then
                For j = 1 To points.Count - 1
                    Dim prev = points(j - 1)
                    Dim curr = points(j)

                    If prev.X = curr.X Then
                        If mouse.Y > Math.Min(prev.Y, curr.Y) And mouse.Y < Math.Max(prev.Y, curr.Y) Then
                            currentStroke = strokes(i)
                            currentPoint = Nothing
                            Return
                        End If
                    End If

                    If prev.Y = curr.Y Then
                        If mouse.X > Math.Min(prev.X, curr.X) And mouse.X < Math.Max(prev.X, curr.X) Then
                            currentStroke = strokes(i)
                            currentPoint = Nothing
                            Return
                        End If
                    End If

                    If Not WithinBounds(mouse, prev, curr) Then
                        Continue For
                    End If

                    Dim projection = Project(prev, curr, mouse)
                    Dim length = FindDistance(mouse, projection)
                    If length < lineCollision Then
                        currentStroke = strokes(i)
                        currentPoint = Nothing
                        Return
                    End If
                Next
            End If
        Next

        ' Else deselect
        Deselect()

        movePoints = False
    End Sub

    Private Sub Deselect()
        currentStroke = Nothing
        currentPoint = Nothing
    End Sub
#End Region

#Region "Additional Hotkeys"
    Protected Overrides Function ProcessCmdKey(ByRef message As Message, keys As Keys) As Boolean
        Select Case keys
            Case Keys.Back, Keys.Delete
                If CheckBox2.Checked Then
                    If currentPoint Is Nothing Then
                        DeleteCurrentStroke()
                    ElseIf Not currentStroke Is Nothing Then
                        DeleteCurrentPoint()
                    End If

                    UpdateAll()
                    Return True
                End If

            Case Keys.Escape
                Deselect()
                UpdateAll()
                Return True

            Case Keys.A
                RadioButton1.Checked = True
                Return True

            Case Keys.S
                RadioButton2.Checked = True
                Return True

            Case Keys.D
                CheckBox1.Checked = Not CheckBox1.Checked
                Return True

            Case Keys.F
                CheckBox2.Checked = Not CheckBox2.Checked
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

        UpdateAll()
    End Sub
#End Region

#Region "Panel"
    Private Sub Panel1_MouseDown(sender As Object, e As MouseEventArgs) Handles Panel1.MouseDown
        If Not CheckBox2.Checked Then
            Return
        End If

        If RadioButton1.Checked Then
            Dim deselected = False
            If currentStroke Is Nothing Then
                deselected = True
                currentStroke = strokes.Last()
            End If

            Dim points = currentStroke.bezier.points
            If points.Count < 1 Then
                points.Add(e.Location)
            Else
                If ModifierKeys.HasFlag(Keys.Control) And Not deselected Then
                    points.Add(e.Location)
                Else
                    Dim targetIndex As Integer = FindCurrentStrokeIndex()
                    currentStroke = New Stroke()
                    currentStroke.bezier.points.Add(e.Location)
                    strokes.Insert(targetIndex + 1, currentStroke)
                End If
            End If
        ElseIf RadioButton2.Checked Then
            MakeSelection(e.Location)
        End If

        UpdateAll()
    End Sub

    Private Sub Panel1_MouseMove(sender As Object, e As MouseEventArgs) Handles Panel1.MouseMove
        If movePoints Then
            If currentPoint Is Nothing Then
                Dim move = New Point(e.Location.X - prevMousePos.X, e.Location.Y - prevMousePos.Y)
                For i = 0 To currentStroke.bezier.points.Count - 1
                    Dim moved = New Point(currentStroke.bezier.points(i).X + move.X - prevMoveAmount.X,
                                          currentStroke.bezier.points(i).Y + move.Y - prevMoveAmount.Y)
                    currentStroke.bezier.points(i) = moved
                Next
                prevMoveAmount = move
                Panel1.Invalidate()
            Else
                currentStroke.bezier.points(currentPointIndex) = e.Location
                currentPoint = currentStroke.bezier.points(currentPointIndex)
                ' Only invalidate instead of UpdateAll() for speed
                'UpdateAll()
                Panel1.Invalidate()
            End If
        End If
    End Sub

    Private Sub Panel1_MouseUp(sender As Object, e As MouseEventArgs) Handles Panel1.MouseUp
        movePoints = False
    End Sub

    Private Sub Panel1_Paint(sender As Object, e As PaintEventArgs) Handles Panel1.Paint
        For i = 50 To 450 Step 50
            e.Graphics.DrawLine(penGrid, New Point(0, i), New Point(500, i))
            e.Graphics.DrawLine(penGrid, New Point(i, 0), New Point(i, 500))
        Next

        Dim paintPen As Pen
        Dim paintBrush As SolidBrush

        For Each stroke In strokes
            If Not currentPoint Is Nothing Then
                paintPen = penBlack
                paintBrush = brushBlack
            ElseIf currentStroke Is stroke Then
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
                    Dim length = FindDistance(stroke.bezier.points(0), measure)
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
                    If startPoint = Nothing Then
                        startPoint = point
                    Else
                        e.Graphics.DrawLine(paintPen, startPoint, point)
                        startPoint = point
                    End If
                Next

                For Each point In stroke.bezier.points
                    Dim rect As New Rectangle(point.X - brushRadius, point.Y - brushRadius, brushRadius * 2, brushRadius * 2)
                    If currentPoint = point Then
                        e.Graphics.FillRectangle(brushRed, rect)
                    Else
                        e.Graphics.FillRectangle(paintBrush, rect)
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
            currentPoint = currentStroke.bezier.points(e.Node.Index)
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

            UpdateAll()
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
            UpdateAll()
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

    Private Sub DeleteToolStripMenuItem_Click(sender As Object, e As EventArgs)
        DeleteCurrentStroke()
    End Sub
#End Region

#Region "Yolo"
    Private Sub Panel2_Paint(sender As Object, e As PaintEventArgs) Handles Panel2.Paint
        e.Graphics.DrawLine(penWhite, 0, 0, 0, 5)
        e.Graphics.DrawLine(penWhite, 0, 5, 254, 5)
        e.Graphics.DrawLine(penWhite, 253, 0, 253, 5)
    End Sub
#End Region

#Region "Configuration"
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

    Private Sub RadioButton1_CheckedChanged(sender As Object, e As EventArgs) Handles RadioButton1.CheckedChanged
        UpdateAll()
    End Sub

    Private Sub RadioButton2_CheckedChanged(sender As Object, e As EventArgs) Handles RadioButton2.CheckedChanged
        UpdateAll()
    End Sub
#End Region
End Class
