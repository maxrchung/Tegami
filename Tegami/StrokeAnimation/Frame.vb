Public Class Frame
    Public strokes As New List(Of Stroke)
    Public colorWhites As New List(Of ColorRectangle)
    Public colorBlacks As New List(Of ColorRectangle)
    Public timeSpan As New TimeSpan

    Sub New(strokes As List(Of Stroke), colorWhites As List(Of ColorRectangle), colorBlacks As List(Of ColorRectangle), timeSpan As TimeSpan)
        Me.strokes = strokes
        Me.colorWhites = colorWhites
        Me.colorBlacks = colorBlacks
        Me.timeSpan = timeSpan
    End Sub

    Sub New()
    End Sub

    Public Sub AddLine(line As Line)
        Dim stroke As New Stroke(New Point(line.X1, line.Y1),
                                 New Point(line.X2, line.Y2))
        strokes.Add(stroke)
    End Sub

    Public Sub RemoveLine(line As Line)
        Dim stroke As New Stroke(New Point(line.X1, line.Y1),
                                 New Point(line.X2, line.Y2))
        strokes.Remove(stroke)
    End Sub

    Public Sub AddColorWhite(rectangle As Rectangle)
        Dim colorRectangle As ColorRectangle = GetColorRectangle(rectangle)
        colorWhites.Add(colorRectangle)
    End Sub

    Public Sub RemoveColorWhite(rectangle As Rectangle)
        Dim colorRectangle As ColorRectangle = GetColorRectangle(rectangle)
        colorWhites.Remove(colorRectangle)
    End Sub

    Public Sub AddColorBlack(rectangle As Rectangle)
        Dim colorRectangle As ColorRectangle = GetColorRectangle(rectangle)
        colorBlacks.Add(colorRectangle)
    End Sub

    Public Sub RemoveColorBlack(rectangle As Rectangle)
        Dim colorRectangle As ColorRectangle = GetColorRectangle(rectangle)
        colorBlacks.Remove(colorRectangle)
    End Sub

    Private Function GetColorRectangle(rectangle As Rectangle) As ColorRectangle
        Dim rect As New Rect(Canvas.GetLeft(rectangle),
                     Canvas.GetTop(rectangle),
                     rectangle.Width,
                     rectangle.Height)
        Dim rotateTransform As RotateTransform = rectangle.RenderTransform
        Dim rotation As Double = rotateTransform.Angle
        Dim fill As SolidColorBrush = rectangle.Fill
        Dim colorRectangle As New ColorRectangle(rect, rotation, fill)

        Return colorRectangle
    End Function
End Class
