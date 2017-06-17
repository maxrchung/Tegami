
Public Class Form1
    Dim strokes As List(Of Stroke) = New List(Of Stroke) From {New Stroke()}
    Dim currentStroke As Stroke = strokes(0)
    Dim penWidth As Integer = 2
    Dim penBlack As Pen = New Pen(Color.Black, penWidth)
    Dim penRed As Pen = New Pen(Color.Red, penWidth)
    Dim brushBlack As SolidBrush = New SolidBrush(Color.Black)
    Dim brushRed As SolidBrush = New SolidBrush(Color.Red)
    Dim brushRadius As Integer = 4
    Dim graphics As Graphics

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        graphics = Panel1.CreateGraphics()
    End Sub

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
                graphics.FillEllipse(paintBrush, rect)

                If startPoint = Nothing Then
                    startPoint = point
                Else
                    graphics.DrawLine(paintPen, startPoint, point)
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
                TreeView1.Nodes(index).Nodes.Add("(" & point.X & ", " & point.Y & ")")
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
End Class
