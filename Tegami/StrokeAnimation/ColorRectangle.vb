Public Class ColorRectangle
    Public rect As New Rect
    Public rotation As New Integer
    Public color As New SolidColorBrush

    Sub New(rect As Rect, rotation As Integer, color As SolidColorBrush)
        Me.rect = rect
        Me.rotation = rotation
        Me.color = color
    End Sub

    Sub New()
    End Sub
End Class
