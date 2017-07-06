Public Class Frame
    Public strokes As New List(Of Stroke)
    Public colorWhites As New List(Of ColorRectangle)
    Public colorBlacks As New List(Of ColorRectangle)
    Public timeSpan As New TimeSpan

    Private timeSpanStringProp As String
    Public Property TimeSpanString() As String
        Get
            Return timeSpanStringProp
        End Get
        Set(ByVal value As String)
            timeSpanStringProp = value
        End Set
    End Property

    Private countProp As Integer
    Public Property Count() As Integer
        Get
            Return countProp
        End Get
        Set(ByVal value As Integer)
            countProp = value
        End Set
    End Property

    Sub New(strokes As List(Of Stroke), colorWhites As List(Of ColorRectangle), colorBlacks As List(Of ColorRectangle), timeSpan As TimeSpan)
        Me.strokes = strokes
        Me.colorWhites = colorWhites
        Me.colorBlacks = colorBlacks
        Me.timeSpan = timeSpan
    End Sub

    Sub New(timeSpan As TimeSpan)
        Me.timeSpan = timeSpan
        TimeSpanString = MainWindow.FormatTime(timeSpan)
    End Sub

    Sub New()
    End Sub

    Public Sub AddLine(line As Line)
        Dim stroke As New Stroke(New Point(line.X1, line.Y1),
                                 New Point(line.X2, line.Y2))
        strokes.Add(stroke)
        Count += 1
    End Sub

    Public Sub RemoveLine(line As Line)
        Dim stroke As New Stroke(New Point(line.X1, line.Y1),
                                 New Point(line.X2, line.Y2))
        For Each compare As Stroke In strokes
            If compare.first.Equals(stroke.first) AndAlso compare.second.Equals(stroke.second) Then
                strokes.Remove(compare)
                Return
            End If
        Next
        Count -= 1
    End Sub

    Public Sub AddColorWhite(rectangle As Rectangle)
        Dim colorRectangle As ColorRectangle = GetColorRectangle(rectangle)
        colorWhites.Add(colorRectangle)
        count += 1
    End Sub

    Public Sub RemoveColorWhite(rectangle As Rectangle)
        Dim colorRectangle As ColorRectangle = GetColorRectangle(rectangle)
        For Each compare As ColorRectangle In colorWhites
            If compare.rect.Equals(colorRectangle.rect) AndAlso compare.rotation.Equals(colorRectangle.rotation) Then
                colorWhites.Remove(compare)
                Return
            End If
        Next
        count -= 1
    End Sub

    Public Sub AddColorBlack(rectangle As Rectangle)
        Dim colorRectangle As ColorRectangle = GetColorRectangle(rectangle)
        colorBlacks.Add(colorRectangle)
        count += 1
    End Sub

    Public Sub RemoveColorBlack(rectangle As Rectangle)
        Dim colorRectangle As ColorRectangle = GetColorRectangle(rectangle)
        For Each compare As ColorRectangle In colorBlacks
            If compare.rect.Equals(colorRectangle.rect) AndAlso compare.rotation.Equals(colorRectangle.rotation) Then
                colorWhites.Remove(compare)
                Return
            End If
        Next
        count -= 1
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
