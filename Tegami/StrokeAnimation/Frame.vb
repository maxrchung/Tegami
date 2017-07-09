Imports System.Xml
Imports System.Xml.Serialization

Public Class Frame
    Public strokes As New List(Of Stroke)
    Public colorRectangles As New List(Of ColorRectangle)
    <XmlIgnore>
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

    Public Property MyTimeSpan() As String
        Get
            Return timeSpan.ToString()
        End Get
        Set(value As String)
            timeSpan = TimeSpan.Parse(value)
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

    Sub New(strokes As List(Of Stroke), colorRectangles As List(Of ColorRectangle), timeSpan As TimeSpan)
        Me.strokes = strokes
        Me.colorRectangles = colorRectangles
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
                Count -= 1
                Return
            End If
        Next
    End Sub

    Public Sub AddColorRectangle(rectangle As Rectangle)
        Dim colorRectangle As ColorRectangle = GetColorRectangle(rectangle)
        colorRectangles.Add(colorRectangle)
        Count += 1
    End Sub

    Public Sub RemoveColorRectangle(rectangle As Rectangle)
        Dim colorRectangle As ColorRectangle = GetColorRectangle(rectangle)
        For Each compare As ColorRectangle In colorRectangles
            If compare.simpleRect.GetRect().Equals(colorRectangle.simpleRect.GetRect()) AndAlso compare.rotation.Equals(colorRectangle.rotation) Then
                colorRectangles.Remove(compare)
                Count -= 1
                Return
            End If
        Next
    End Sub

    Private Function GetColorRectangle(rectangle As Rectangle) As ColorRectangle
        Dim rect As New Rect(Canvas.GetLeft(rectangle),
                     Canvas.GetTop(rectangle),
                     rectangle.Width,
                     rectangle.Height)
        Dim rotateTransform As RotateTransform = rectangle.RenderTransform
        Dim rotation As Double = rotateTransform.Angle
        Dim fill As SolidColorBrush = rectangle.Fill
        Dim colorRectangle As New ColorRectangle(rect, rotation)

        Return colorRectangle
    End Function
End Class
