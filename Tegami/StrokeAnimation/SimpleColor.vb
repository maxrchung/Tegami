' Simpler version of SolidColorBrush for XmlSerializer
Public Class SimpleColor
    Public white As Boolean = False

    Sub New()

    End Sub

    Sub New(brush As SolidColorBrush)
        If brush Is Brushes.White Then
            white = True
        Else
            white = False
        End If
    End Sub

    Public Function GetColor() As SolidColorBrush
        If white = True Then
            Return Brushes.White
        Else
            Return Brushes.Black
        End If
    End Function
End Class
