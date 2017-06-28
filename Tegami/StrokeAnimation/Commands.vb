
Public Module Commands
    Public ReadOnly Import As New RoutedUICommand("Import", "Import", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.I, ModifierKeys.Control)}))
    Public ReadOnly PlayPause As New RoutedUICommand("PlayPause", "PlayPause", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.Space, ModifierKeys.None)}))
    Public ReadOnly Draw As New RoutedUICommand("Draw", "Draw", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.Q, ModifierKeys.Control)}))
    Public ReadOnly ColorWhite As New RoutedUICommand("ColorWhite", "ColorWhite", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.W, ModifierKeys.Control)}))
    Public ReadOnly ColorBlack As New RoutedUICommand("ColorBlack", "ColorBlack", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.E, ModifierKeys.Control)}))
End Module
