Public Class Bezier
    Public points As New List(Of Point)
    Public Sub New(ppoints As List(Of Point))
        points = ppoints
    End Sub

    Public Function FindPosition(pdelta As Double) As Point
        Return reduceCurve(points, pdelta)
    End Function

    Private Function reduceCurve(ppoints As List(Of Point), pdelta As Double) As Point
        If ppoints.Count = 1 Then
            Return ppoints(0)
        Else
            Dim reduce = New List(Of Point)(ppoints.Count - 1)
            For i = 0 To (ppoints.Count - 2)
                Dim x = ppoints(i).X * (1 - pdelta) + ppoints(i + 1).X * pdelta
                Dim y = ppoints(i).Y * (1 - pdelta) + ppoints(i + 1).Y * pdelta
                reduce.Add(New Point(x, y))
            Next
            Return reduceCurve(reduce, pdelta)
        End If
    End Function
End Class
