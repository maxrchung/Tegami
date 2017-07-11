Public Class ColorRectangle
    Public simpleRect As New SimpleRect
    Public rotation As New Integer

    Sub New(rect As Rect, rotation As Integer)
        simpleRect = New SimpleRect(rect)
        Me.rotation = rotation
    End Sub

    Sub New()
    End Sub
End Class
