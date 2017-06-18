Public Class Stroke
    Public points As New List(Of Point)
    Public Shared idCounter As Integer = 0
    Public id As Integer

    Sub New()
        id = idCounter
        idCounter += 1
    End Sub

    Sub New(givenID As Integer)
        id = givenID
    End Sub

    Function Clone() As Stroke
        Return MemberwiseClone()
    End Function
End Class
