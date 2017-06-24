Imports Microsoft.Win32
Imports System.Windows.Threading

Class MainWindow
    Dim playState As PlayState = PlayState.None
    Dim timeSpanTotal As TimeSpan
    Dim progressTimer As DispatcherTimer

    Dim currentPoint As Point
    Dim lineTimer As DispatcherTimer
    Dim currentLine As Line

    Dim currentTool As Tool = Tool.Pointer

    Private Function FormatTime(timeSpan As TimeSpan)
        Dim time As String = String.Format("{0}:{1}:{2}", timeSpan.Minutes.ToString("D2"), timeSpan.Seconds.ToString("D2"), timeSpan.Milliseconds.ToString("D3"))
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
        timeSpanTotal = Player.NaturalDuration.TimeSpan

        progressTimer = New DispatcherTimer(DispatcherPriority.Render)
        progressTimer.Interval = TimeSpan.FromSeconds(0.001)
        AddHandler progressTimer.Tick, AddressOf progressTimer_Tick
        progressTimer.Start()
    End Sub

    Private Sub progressTimer_Tick(sender As Object, e As EventArgs)
        If Player.NaturalDuration.TimeSpan.TotalSeconds > 0 Then
            If timeSpanTotal.TotalSeconds > 0 Then
                Progress.Value = Player.Position.TotalSeconds / timeSpanTotal.TotalSeconds

                CurrentTime.Content = FormatTime(Player.Position)
                TotalTime.Content = FormatTime(timeSpanTotal)
            End If
        End If
    End Sub

    Private Sub Progress_Loaded(sender As Object, e As EventArgs)
        AddHandler Progress.MouseDown, AddressOf Progress_MouseDown
    End Sub

    Private Sub Progress_MouseDown(sender As Object, e As MouseEventArgs)
        Dim ratio As Double = e.GetPosition(Progress).X / Progress.ActualWidth
        Progress.Value = ratio * Progress.Maximum
        Player.Position = New TimeSpan(0, 0, ratio * timeSpanTotal.TotalSeconds)
    End Sub

    Private Sub VideoDisplay_Checked(sender As Object, e As EventArgs)
        Player.Visibility = Visibility.Visible
        VideoOpacity.IsEnabled = True
    End Sub

    Private Sub VideoDisplay_Unchecked(sender As Object, e As EventArgs)
        Player.Visibility = Visibility.Hidden
        VideoOpacity.IsEnabled = False
    End Sub

    Private Sub VideoOpacity_ValueChanged(sender As Object, e As EventArgs)
        Player.Opacity = VideoOpacity.Value
    End Sub

    Private Sub Panel_Loaded(sender As Object, e As EventArgs)
        AddHandler Panel.MouseDown, AddressOf Panel_MouseDown
        AddHandler Panel.MouseUp, AddressOf Panel_MouseUp
    End Sub

    Private Sub Panel_MouseDown(sender As Object, e As MouseEventArgs)
        currentPoint = e.GetPosition(Panel)
        currentLine = ConstructLine(currentPoint, currentPoint)
        Lines.Children.Add(currentLine)

        lineTimer = New DispatcherTimer(DispatcherPriority.Render)
        lineTimer.Interval = TimeSpan.FromSeconds(0.001)
        AddHandler lineTimer.Tick, AddressOf lineTimer_Tick
        lineTimer.Start()
    End Sub

    Private Sub Panel_MouseUp(sender As Object, e As MouseEventArgs)
        Dim endPoint = e.GetPosition(Panel)
        currentLine = ConstructLine(currentPoint, endPoint)
    End Sub

    Private Sub lineTimer_Tick(sender As Object, e As EventArgs)
        Dim point As Point = Mouse.GetPosition(Panel)
        currentLine.X2 = point.X
        currentLine.Y2 = point.Y
    End Sub

    Private Function ConstructLine(first As Point, second As Point) As Line
        Dim line As New Line()
        line.X1 = first.X
        line.Y1 = first.Y
        line.X2 = second.X
        line.Y2 = second.Y
        line.Stroke = Brushes.Black
        line.Visibility = Lines.Visibility
        line.Opacity = Lines.Opacity
        Return line
    End Function

    Private Sub LinesDisplay_Checked(sender As Object, e As EventArgs)
        Lines.Visibility = Visibility.Visible
        LinesOpacity.IsEnabled = True
    End Sub

    Private Sub LinesDisplay_Unchecked(sender As Object, e As EventArgs)
        Lines.Visibility = Visibility.Hidden
        LinesOpacity.IsEnabled = False
    End Sub

    Private Sub LinesOpacity_ValueChanged(sender As Object, e As EventArgs)
        Lines.Opacity = LinesOpacity.Value
    End Sub

    Private Sub Volume_Checked(sender As Object, e As EventArgs)
        Player.IsMuted = False
        VolumeLevel.IsEnabled = True
    End Sub

    Private Sub Volume_Unchecked(sender As Object, e As EventArgs)
        Player.IsMuted = True
        VolumeLevel.IsEnabled = False
    End Sub

    Private Sub VolumeLevel_ValueChanged(sender As Object, e As EventArgs)
        Player.Volume = VolumeLevel.Value
    End Sub
End Class