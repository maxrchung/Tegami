Public Class Frame
    Public lines As List(Of Stroke)
    Public timeSpan As TimeSpan

    Sub New(lines As List(Of Stroke), timeSpan As TimeSpan)
        Me.lines = lines
        Me.timeSpan = timeSpan
    End Sub

    Sub New()
    End Sub
End Class
