Public Class ColorRectangle
    Public simpleRect As New SimpleRect
    Public rotation As New Integer
    Public simpleColor As New SimpleColor

    Sub New(rect As Rect, rotation As Integer, color As SolidColorBrush)
        Me.simpleRect = New SimpleRect(rect)
        Me.rotation = rotation
        Me.simpleColor = New SimpleColor(color)
    End Sub

    Sub New()
    End Sub
End Class
