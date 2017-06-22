Imports Microsoft.Win32
Imports System.Windows.Threading

Class MainWindow
    Dim playState As PlayState = PlayState.None
    Dim timeSpan As TimeSpan
    Dim dispatcherTimer As DispatcherTimer

    Private Function FormatTime(minutes As Integer, seconds As Integer)
        Dim time As String = String.Format("{0}:{1}", minutes.ToString("D2"), seconds.ToString("D2"))
        Return time
    End Function

    Private Sub New_Click(sender As Object, e As RoutedEventArgs)

    End Sub

    Private Sub Import_Click(sender As Object, e As RoutedEventArgs)
        Dim dialog As New OpenFileDialog
        If dialog.ShowDialog() = True Then
            Player.Source = New Uri(dialog.FileName)
        End If
    End Sub

    Private Sub Open_Click(sender As Object, e As RoutedEventArgs)

    End Sub

    Private Sub Save_Click(sender As Object, e As RoutedEventArgs)

    End Sub

    Private Sub Player_PlayPause(sender As Object, e As RoutedEventArgs)
        Select Case playState
            Case PlayState.None
                playState = PlayState.Play
                Player.Play()
                Return
            Case PlayState.Play
                playState = PlayState.Pause
                Player.Pause()
                Return
            Case PlayState.Pause
                playState = PlayState.Play
                Player.Play()
                Return
        End Select
    End Sub

    ' https://stackoverflow.com/questions/10208959/binding-mediaelement-to-slider-position-in-wpf
    Private Sub Player_MediaOpened(sender As Object, e As RoutedEventArgs)
        timeSpan = Player.NaturalDuration.TimeSpan

        dispatcherTimer = New DispatcherTimer()
        dispatcherTimer.Interval = TimeSpan.FromSeconds(1)
        AddHandler dispatcherTimer.Tick, AddressOf timer_Tick
        dispatcherTimer.Start()
    End Sub

    Private Sub timer_Tick(sender As Object, e As EventArgs)
        If Player.NaturalDuration.TimeSpan.TotalSeconds > 0 Then
            If timeSpan.TotalSeconds > 0 Then
                Progress.Value = Player.Position.TotalSeconds / timeSpan.TotalSeconds

                CurrentTime.Content = FormatTime(Player.Position.Minutes, Player.Position.Seconds)
                TotalTime.Content = FormatTime(timeSpan.Minutes, timeSpan.Seconds)
            End If
        End If
    End Sub

    Private Sub Progress_Load(sender As Object, e As EventArgs)
        AddHandler Progress.MouseDown, AddressOf Progress_Click
    End Sub

    Private Sub Progress_Click(sender As Object, e As MouseEventArgs)
        Dim ratio As Double = e.GetPosition(Progress).X / Progress.ActualWidth
        Progress.Value = ratio * Progress.Maximum
        Player.Position = New TimeSpan(0, 0, ratio * timeSpan.TotalSeconds)
    End Sub
End Class