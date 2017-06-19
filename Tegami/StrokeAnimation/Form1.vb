Imports WMPLib

Public Class Form1
    Protected Overrides Function ProcessCmdKey(ByRef message As Message, keys As Keys) As Boolean
        Select Case keys
            Case Keys.Space
                If AxWindowsMediaPlayer1.playState = WMPPlayState.wmppsPlaying Then
                    AxWindowsMediaPlayer1.Ctlcontrols.pause()
                    Return True
                Else
                    AxWindowsMediaPlayer1.Ctlcontrols.play()
                    Return True
                End If
            Case Keys.Left
                Return True
            Case Keys.Right
                Return True
        End Select
        Return MyBase.ProcessCmdKey(message, keys)
    End Function

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        OpenFileDialog2.Title = "Import"
    End Sub

    Private Sub ImportToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles ImportToolStripMenuItem.Click
        Dim result As DialogResult = OpenFileDialog2.ShowDialog()
        If result = DialogResult.OK Then
            AxWindowsMediaPlayer1.URL = OpenFileDialog2.FileName
        End If
    End Sub

    Private Sub CheckBox1_CheckedChanged(sender As Object, e As EventArgs) Handles CheckBox1.CheckedChanged
        If CheckBox1.Checked = True Then
            AxWindowsMediaPlayer1.Show()
        Else
            AxWindowsMediaPlayer1.Hide()
        End If
    End Sub
End Class
