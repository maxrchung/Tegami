

Public Class Form1
    Dim strokes = New List(Of Stroke) From {New Stroke()}
    Dim stroke = strokes(0)
    Dim penWidth = 2
    Dim penBlack = New Pen(Color.Black, penWidth)
    Dim penRed = New Pen(Color.Red, penWidth)
    Dim brushBlack = New SolidBrush(Color.Black)
    Dim brushRed = New SolidBrush(Color.Red)
    Dim brushRadius As Integer = 5

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
    End Sub

    Private Sub FileToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles FileToolStripMenuItem.Click

    End Sub

    Private Sub TreeView1_AfterSelect(sender As Object, e As TreeViewEventArgs) Handles TreeView1.AfterSelect

    End Sub

    Private Sub GroupBox1_Enter(sender As Object, e As EventArgs) Handles GroupBox1.Enter

    End Sub

    Private Sub Panel1_Paint(sender As Object, e As PaintEventArgs) Handles Panel1.Paint
        Dim startPoint As Point
        For Each stroke In strokes
            For Each point In stroke.points
                Dim rect As New Rectangle(point.X - brushRadius, point.Y - brushRadius, brushRadius * 2, brushRadius * 2)
                e.Graphics.FillEllipse(brushBlack, rect)

                If startPoint = Nothing Then
                    startPoint = point
                Else
                    e.Graphics.DrawLine(penBlack, startPoint, point)
                    startPoint = point
                End If
            Next
        Next
    End Sub

    Private Sub Panel1_Click(sender As Object, e As MouseEventArgs) Handles Panel1.MouseClick
        stroke.points.Add(e.Location)

        Panel1.Invalidate()
    End Sub

    Private Sub OpenToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles OpenToolStripMenuItem.Click

    End Sub

    Private Sub ToolStripButton2_Click(sender As Object, e As EventArgs) Handles ToolStripButton2.Click

    End Sub

    Private Sub ToolStrip1_ItemClicked(sender As Object, e As ToolStripItemClickedEventArgs) Handles ToolStrip1.ItemClicked

    End Sub

    Private Sub ToolStripButton1_Click(sender As Object, e As EventArgs) Handles ToolStripButton1.Click

    End Sub
End Class
