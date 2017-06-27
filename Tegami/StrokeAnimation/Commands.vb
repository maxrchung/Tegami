
Public Module Commands
    Public ReadOnly Import As New RoutedUICommand("Import", "Import", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.I, ModifierKeys.Control)}))
    Public ReadOnly PlayPause As New RoutedUICommand("PlayPause", "PlayPause", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.Space, ModifierKeys.None)}))
End Module
