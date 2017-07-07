Imports Microsoft.Win32
Imports System.Windows.Threading
Imports System.Xml.Serialization
Imports System.IO

Class MainWindow
#Region "Variables"
    Dim DToR = Math.PI / 180

    Dim playState As PlayState = PlayState.None
    Dim timeSpanTotal As New TimeSpan
    Dim progressTimer As New DispatcherTimer

    Dim offset As Double = 2222
    Dim songLength As Double = 300000
    Dim bpm As Double = 77
    Dim mpb As Double = 1 / bpm
    Dim spb As Double = mpb * 60
    Dim mspb As Double = spb * 1000
    Dim mspf As Double = mspb / 4

    Dim currentPoint As New Point
    Dim lineTimer As New DispatcherTimer
    Dim currentLine As New Line
    Dim lineMaxThreshold As Integer = 25
    Dim drawThreshold As Integer = 9
    Dim rectTimer As New DispatcherTimer
    Dim currentRect As New Rectangle
    Dim rotationAngle As Integer = 5

    Dim currentTool As Tool = Tool.Draw

    Dim frames As New List(Of Frame)
    Dim currentFrame As New Frame()
    Dim prevFrame As New Frame()
#End Region

#Region "Helper Functions"
    Private Function ConstructLine(first As Point, second As Point) As Line
        Dim line As New Line()
        line.X1 = first.X
        line.Y1 = first.Y
        line.X2 = second.X
        line.Y2 = second.Y
        line.Stroke = Brushes.Black
        Return line
    End Function

    Private Function FindFrameIndex() As Integer
        For index As Integer = 0 To frames.Count - 1
            If currentFrame Is frames(index) Then
                Return index
            End If
        Next
        Throw New Exception("Could not find frame index")
    End Function

    Private Function FindFrameIndex(frame As Frame) As Integer
        For index As Integer = 0 To frames.Count - 1
            If frame Is frames(index) Then
                Return index
            End If
        Next
        Throw New Exception("Could not find frame index from existing frame")
    End Function

    Private Function FindFrame(timeSpan As TimeSpan) As Frame
        ' Yolo accounting for floating point errors?
        timeSpan += New TimeSpan(0, 0, 0, 0, 1)
        Dim prevFrame As Frame = frames.First()
        For Each frame As Frame In frames
            If frame.timeSpan > timeSpan Then
                Return prevFrame
            End If
            prevFrame = frame
        Next
        Throw New Exception("Could not find next frame")
    End Function

    Public Shared Function FormatTime(timeSpan As TimeSpan)
        Dim time As String = String.Format("{0}:{1}:{2}", timeSpan.Minutes.ToString("D2"), timeSpan.Seconds.ToString("D2"), timeSpan.Milliseconds.ToString("D3"))
        Return time
    End Function

    ' http//www.vcskicks.com/code-snippet/point-projection.php
    Private Function Project(lineStart As Point, lineEnd As Point, toProject As Point) As Point
        Dim m As Double = (lineEnd.Y - lineStart.Y) / (lineEnd.X - lineStart.X)
        Dim b As Double = lineStart.Y - (m * lineStart.X)

        Dim x As Double = (m * toProject.Y + toProject.X - m * b) / (m * m + 1)
        Dim y As Double = (m * m * toProject.Y + m * toProject.X + b) / (m * m + 1)

        Return New Point(x, y)
    End Function

    Private Sub LoadFrame(frame As Frame)
        If currentFrame Is frame Then
            Return
        End If

        PreviousColorWhites.Children.Clear()
        PreviousColorBlacks.Children.Clear()
        PreviousLines.Children.Clear()

        Dim frameIndex As Integer = FindFrameIndex(frame)
        If Not frameIndex = 0 Then
            Dim previousFrame As Frame = frames(frameIndex - 1)
            For Each stroke As Stroke In previousFrame.strokes
                Dim line As Line = ConstructLine(stroke.first, stroke.second)
                PreviousLines.Children.Add(line)
            Next
            For Each colorWhite As ColorRectangle In previousFrame.colorWhites
                Dim rectangle As Rectangle = LoadRectangle(colorWhite)
                rectangle.Fill = colorWhite.simpleColor.GetColor()
                PreviousColorWhites.Children.Add(rectangle)
            Next
            For Each colorBlack As ColorRectangle In previousFrame.colorBlacks
                Dim rectangle As Rectangle = LoadRectangle(colorBlack)
                rectangle.Fill = colorBlack.simpleColor.GetColor()
                PreviousColorBlacks.Children.Add(rectangle)
            Next
        End If

        Lines.Children.Clear()
        ColorWhites.Children.Clear()
        ColorBlacks.Children.Clear()

        currentFrame = frame
        For Each stroke As Stroke In currentFrame.strokes
            Dim line As Line = ConstructLine(stroke.first, stroke.second)
            Lines.Children.Add(line)
        Next
        For Each colorWhite As ColorRectangle In currentFrame.colorWhites
            Dim rectangle As Rectangle = LoadRectangle(colorWhite)
            rectangle.Fill = colorWhite.simpleColor.GetColor()
            ColorWhites.Children.Add(rectangle)
        Next
        For Each colorBlack As ColorRectangle In currentFrame.colorBlacks
            Dim rectangle As Rectangle = LoadRectangle(colorBlack)
            rectangle.Fill = colorBlack.simpleColor.GetColor()
            ColorBlacks.Children.Add(rectangle)
        Next

        FramesView.ScrollIntoView(FramesView.Items(frameIndex))
        FramesView.SelectedValue = FramesView.Items(frameIndex)
    End Sub

    Private Function LoadRectangle(colorRect As ColorRectangle) As Rectangle
        Dim rect As Rect = colorRect.simpleRect.GetRect()
        Dim rectangle As New Rectangle()
        Canvas.SetLeft(rectangle, rect.Left)
        Canvas.SetTop(rectangle, rect.Top)
        rectangle.Width = rect.Width
        rectangle.Height = rect.Height

        Dim rotateTransform As New RotateTransform(0)
        rectangle.RenderTransform = rotateTransform
        rectangle.RenderTransformOrigin = New Point(0.5, 0.5)
        rotateTransform.Angle = colorRect.rotation

        rectangle.Fill = colorRect.simpleColor.GetColor()

        Return rectangle
    End Function

    Private Function TimeSpanMilliseconds(milliseconds As Double) As TimeSpan
        Return New TimeSpan(0, 0, 0, 0, milliseconds)
    End Function
#End Region

#Region "MainWindow"
    Private Sub MainWindow_Loaded(sender As Object, e As EventArgs)
        For time As Double = offset To offset + songLength Step mspf
            Dim frame As New Frame(TimeSpanMilliseconds(time))
            frames.Add(frame)
        Next
        FramesView.ItemsSource = frames
        LoadFrame(frames.First())
    End Sub
#End Region

#Region "File"
    Private Sub New_Click(sender As Object, e As RoutedEventArgs)
        Lines.Children.Clear()
        ColorWhites.Children.Clear()
        ColorBlacks.Children.Clear()

        For Each frame In frames
            If frame.Count > 0 Then
                frame.strokes.Clear()
                frame.colorWhites.Clear()
                frame.colorBlacks.Clear()
                frame.Count = 0
            End If
        Next
        FramesView.Items.Refresh()
    End Sub

    Private Sub Import_Click(sender As Object, e As RoutedEventArgs)
        Dim dialog As New OpenFileDialog
        If dialog.ShowDialog() = True Then
            Player.Source = New Uri(dialog.FileName)
        End If
    End Sub

    Private Sub Open_Click(sender As Object, e As RoutedEventArgs)
        Dim dialog As New OpenFileDialog
        dialog.Filter = "StrokeAnimation files (*.sa)|*.sa|All files (*.*)|*.*"
        If dialog.ShowDialog() = True Then
            Dim fileName As String = dialog.FileName
            Using reader As StreamReader = My.Computer.FileSystem.OpenTextFileReader(dialog.FileName)
                Dim serializer As New XmlSerializer(GetType(List(Of Frame)))
                Dim timeSpan As TimeSpan = currentFrame.timeSpan
                Dim frameIndex As Integer = FindFrameIndex(currentFrame)
                frames = serializer.Deserialize(reader)
                currentFrame = New Frame()
                LoadFrame(frames.First())
                FramesView.ItemsSource = frames
                FramesView.Items.Refresh()
            End Using
        End If
    End Sub

    Private Sub Save_Click(sender As Object, e As RoutedEventArgs)
        Dim dialog As New SaveFileDialog
        dialog.Filter = "StrokeAnimation files (*.sa)|*.sa"
        If dialog.ShowDialog() = True Then
            Using writer As StreamWriter = My.Computer.FileSystem.OpenTextFileWriter(dialog.FileName, False)
                Dim serializer As New XmlSerializer(GetType(List(Of Frame)))
                serializer.Serialize(writer, frames)
                FramesView.Items.Refresh()
            End Using
        End If
    End Sub
#End Region

#Region "Player"
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
#End Region

#Region "Progress"
    Private Sub progressTimer_Tick(sender As Object, e As EventArgs)
        If Player.NaturalDuration.HasTimeSpan AndAlso Player.NaturalDuration.TimeSpan.TotalSeconds > 0 AndAlso timeSpanTotal.TotalSeconds > 0 Then
            Progress.Value = Player.Position.TotalSeconds / timeSpanTotal.TotalSeconds
            CurrentTime.Content = FormatTime(Player.Position)
            TotalTime.Content = FormatTime(timeSpanTotal)

            If Player.Position < frames.First().timeSpan Then
                LoadFrame(frames.First())
            Else
                Dim index As Integer = FindFrameIndex()
                If Not index = frames.Count - 1 Then
                    Dim frame As Frame = FindFrame(Player.Position)
                    LoadFrame(frame)
                End If
            End If
        End If
    End Sub

    Private Sub Progress_Loaded(sender As Object, e As EventArgs)
        AddHandler Progress.MouseDown, AddressOf Progress_MouseDown
    End Sub

    Private Sub Progress_MouseDown(sender As Object, e As MouseButtonEventArgs)
        Dim ratio As Double = e.GetPosition(Progress).X / Progress.ActualWidth
        Progress.Value = ratio * Progress.Maximum
        Player.Position = New TimeSpan(0, 0, ratio * timeSpanTotal.TotalSeconds)
    End Sub
#End Region

#Region "Display"
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

    Private Sub PreviousDisplay_Checked(sender As Object, e As EventArgs)
        PreviousPanel.Visibility = Visibility.Visible
        PreviousOpacity.IsEnabled = True
    End Sub

    Private Sub PreviousDisplay_Unchecked(sender As Object, e As EventArgs)
        PreviousPanel.Visibility = Visibility.Hidden
        PreviousOpacity.IsEnabled = False
    End Sub

    Private Sub PreviousOpacity_ValueChanged(sender As Object, e As EventArgs)
        PreviousPanel.Opacity = PreviousOpacity.Value
    End Sub

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

    Private Sub ColorsOpacity_ValueChanged(sender As Object, e As EventArgs)
        ColorWhites.Opacity = ColorsOpacity.Value
        ColorBlacks.Opacity = ColorsOpacity.Value
    End Sub

    Private Sub ColorsDisplay_Checked(sender As Object, e As EventArgs)
        ColorWhites.Visibility = Visibility.Visible
        ColorWhites.IsEnabled = True
        ColorBlacks.Visibility = Visibility.Visible
        ColorBlacks.IsEnabled = True
    End Sub

    Private Sub ColorsDisplay_Unchecked(sender As Object, e As EventArgs)
        ColorWhites.Visibility = Visibility.Hidden
        ColorWhites.IsEnabled = False
        ColorBlacks.Visibility = Visibility.Hidden
        ColorBlacks.IsEnabled = False
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
#End Region

#Region "Panel"
    Private Sub Panel_Loaded(sender As Object, e As EventArgs)
        AddHandler Panel.MouseDown, AddressOf Panel_MouseDown
        AddHandler Panel.MouseUp, AddressOf Panel_MouseUp
    End Sub

    Private Sub Panel_MouseDown(sender As Object, e As MouseButtonEventArgs)
        currentPoint = e.GetPosition(Panel)
        If e.ChangedButton = MouseButton.Left Then
            If currentTool = Tool.Draw Then
                currentLine = ConstructLine(currentPoint, currentPoint)
                Lines.Children.Add(currentLine)

                lineTimer.Stop()
                lineTimer = New DispatcherTimer(DispatcherPriority.Render)
                lineTimer.Interval = TimeSpan.FromSeconds(0.001)
                AddHandler lineTimer.Tick, AddressOf lineTimer_Tick
                lineTimer.Start()
            Else
                currentRect = New Rectangle()
                currentRect.Width = 0
                currentRect.Height = 0

                Dim rotateTransform As New RotateTransform(0)
                currentRect.RenderTransform = rotateTransform
                currentRect.RenderTransformOrigin = New Point(0.5, 0.5)

                Canvas.SetLeft(currentRect, currentPoint.X)
                Canvas.SetTop(currentRect, currentPoint.Y)

                If currentTool = Tool.ColorWhite Then
                    currentRect.Fill = Brushes.White
                    ColorWhites.Children.Add(currentRect)
                Else
                    currentRect.Fill = Brushes.Black
                    ColorBlacks.Children.Add(currentRect)
                End If

                rectTimer.Stop()
                rectTimer = New DispatcherTimer(DispatcherPriority.Render)
                rectTimer.Interval = TimeSpan.FromSeconds(0.001)
                AddHandler rectTimer.Tick, AddressOf rectTimer_Tick
                rectTimer.Start()
            End If
        ElseIf e.ChangedButton = MouseButton.Right Then
            If currentTool = Tool.Draw Then
                For index As Integer = Lines.Children.Count - 1 To 0 Step -1
                    Dim line As Line = Lines.Children(index)
                    ' Initial check with a single point
                    Dim diff As Vector = currentPoint - New Point(line.X1, line.Y1)
                    If diff.Length < lineMaxThreshold Then
                        currentFrame.RemoveLine(line)
                        Lines.Children.Remove(line)
                        Return
                    End If

                    Dim projected As Point = Project(New Point(line.X1, line.Y1), New Point(line.X2, line.Y2), currentPoint)
                    ' Within bounds
                    If (projected.X < line.X1 AndAlso projected.X < line.X2) OrElse
                       (projected.X > line.X1 AndAlso projected.X > line.X2) OrElse
                       (projected.Y > line.Y1 AndAlso projected.Y > line.Y2) OrElse
                       (projected.Y < line.Y1 AndAlso projected.Y < line.Y2) Then
                        Return
                    End If

                    Dim lengthSquared As Double = (currentPoint - projected).LengthSquared
                    If lengthSquared < lineMaxThreshold Then
                        currentFrame.RemoveLine(line)
                        Lines.Children.Remove(line)
                        Return
                    End If
                Next
            Else
                Dim Colors As Canvas
                If currentTool = Tool.ColorWhite Then
                    Colors = ColorWhites
                Else
                    Colors = ColorBlacks
                End If

                For index As Integer = Colors.Children.Count - 1 To 0 Step -1
                    Dim color As Rectangle = Colors.Children(index)

                    ' Rotate point to rectangle's rotation, check Contains
                    Dim center As New Point(Canvas.GetLeft(color) + color.Width / 2, Canvas.GetTop(color) + color.Height / 2)
                    Dim rotateTransform As RotateTransform = color.RenderTransform
                    Dim rotate As Vector = RotateVector(currentPoint - center, rotateTransform.Angle)
                    Dim rect As New Rect(Canvas.GetLeft(color), Canvas.GetTop(color), color.Width, color.Height)
                    If rect.Contains(rotate + center) Then
                        If currentTool = Tool.ColorWhite Then
                            currentFrame.RemoveColorWhite(color)
                        Else
                            currentFrame.RemoveColorBlack(color)
                        End If
                        Colors.Children.Remove(color)
                        Return
                    End If
                Next
            End If
        End If
    End Sub

    Private Function RotateVector(vector As Vector, degrees As Integer)
        Dim radians As Double = -degrees * DToR
        Dim ca As Double = Math.Cos(radians)
        Dim sa As Double = Math.Sin(radians)
        Dim rotate As New Vector(ca * vector.X - sa * vector.Y,
                                 sa * vector.X - ca * vector.Y)
        Return rotate
    End Function

    Private Sub Panel_MouseWheel(sender As Object, e As MouseWheelEventArgs)
        If Not currentTool = Tool.Draw AndAlso Mouse.LeftButton = MouseButtonState.Pressed Then
            Dim rotateTransform As RotateTransform = currentRect.RenderTransform
            If e.Delta > 0 Then
                rotateTransform.Angle -= rotationAngle
            Else
                rotateTransform.Angle += rotationAngle
            End If
        End If
    End Sub

    Private Sub Panel_MouseUp(sender As Object, e As MouseButtonEventArgs)
        If e.ChangedButton = MouseButton.Left Then
            If currentTool = Tool.Draw Then
                Dim currentLinePos As Vector = New Point(currentLine.X2, currentLine.Y2) - New Point(currentLine.X1, currentLine.Y1)
                If currentLinePos.LengthSquared < drawThreshold Then
                    Lines.Children.Remove(currentLine)
                Else
                    currentFrame.AddLine(currentLine)
                End If
                lineTimer.Stop()
            Else
                If currentRect.Width * currentRect.Height < drawThreshold Then
                    If currentTool = Tool.ColorWhite Then
                        ColorWhites.Children.Remove(currentRect)
                    Else
                        ColorBlacks.Children.Remove(currentRect)
                    End If
                Else
                    If currentTool = Tool.ColorWhite Then
                        currentFrame.AddColorWhite(currentRect)
                    Else
                        currentFrame.AddColorBlack(currentRect)
                    End If
                End If
                rectTimer.Stop()
            End If
        End If

        ' Probably the simplest way to refresh everything zzz
        FramesView.Items.Refresh()
    End Sub
#End Region

#Region "Tools"
    Private Sub Draw_Click(sender As Object, e As RoutedEventArgs)
        currentTool = Tool.Draw
        Draw.IsChecked = True
        ColorWhite.IsChecked = False
        ColorBlack.IsChecked = False
    End Sub

    Private Sub ColorWhite_Click(sender As Object, e As RoutedEventArgs)
        currentTool = Tool.ColorWhite
        Draw.IsChecked = False
        ColorWhite.IsChecked = True
        ColorBlack.IsChecked = False
    End Sub

    Private Sub ColorBlack_Click(sender As Object, e As RoutedEventArgs)
        currentTool = Tool.ColorBlack
        Draw.IsChecked = False
        ColorWhite.IsChecked = False
        ColorBlack.IsChecked = True
    End Sub
#End Region

#Region "Tool Timers"
    Private Sub lineTimer_Tick(sender As Object, e As EventArgs)
        Dim point As Point = Mouse.GetPosition(Panel)
        currentLine.X2 = point.X
        currentLine.Y2 = point.Y
    End Sub

    Private Sub rectTimer_Tick(sender As Object, e As EventArgs)
        Dim point As Point = Mouse.GetPosition(Panel)
        Dim diff As New Point(Math.Abs(point.X - currentPoint.X), Math.Abs(point.Y - currentPoint.Y))
        Canvas.SetLeft(currentRect, currentPoint.X - diff.X)
        Canvas.SetTop(currentRect, currentPoint.Y - diff.Y)
        currentRect.Width = diff.X * 2
        currentRect.Height = diff.Y * 2
    End Sub
#End Region

#Region "FramesView"
    Private Sub FramesView_PreviewMouseDown(sender As Object, e As MouseButtonEventArgs)
        Dim item As ListBoxItem = ItemsControl.ContainerFromElement(FramesView, e.OriginalSource)
        If Not item Is Nothing Then
            item.IsSelected = True
            Dim viewIndex As Integer = FramesView.Items.IndexOf(item.DataContext)
            Dim frameToLoad As Frame = frames(viewIndex)
            LoadFrame(frameToLoad)
            Player.Position = frameToLoad.timeSpan
        End If
    End Sub

    Private Sub PreviousFrame(sender As Object, e As RoutedEventArgs)
        Dim previousIndex As Integer = FindFrameIndex() - 1
        If previousIndex > -1 Then
            LoadFrame(frames(previousIndex))
            Player.Position = frames(previousIndex).timeSpan
        End If
    End Sub

    Private Sub NextFrame(sender As Object, e As RoutedEventArgs)
        Dim nextIndex As Integer = FindFrameIndex() + 1
        If nextIndex < frames.Count Then
            LoadFrame(frames(nextIndex))
            Player.Position = frames(nextIndex).timeSpan
        End If
    End Sub
#End Region

End Class