' Simpler version of Rect for XmlSerializer
Public Class SimpleRect
    Public position As Point
    Public size As Size

    Sub New(position As Point, size As Point)
        Me.position = position
        Me.size = size
    End Sub

    Sub New(rect As Rect)
        position = rect.Location
        size = rect.Size
    End Sub

    Sub New()
    End Sub

    Public Function GetRect() As Rect
        Dim rect As New Rect(position.X, position.Y, size.Width, size.Height)
        Return rect
    End Function
End Class
