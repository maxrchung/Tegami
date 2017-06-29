Public Class Stroke
    Public first As New Point
    Public second As New Point

    Sub New(first As Point, second As Point)
        Me.first = first
        Me.second = first
    End Sub

    Sub New()
    End Sub
End Class
