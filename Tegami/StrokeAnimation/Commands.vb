
Public Module Commands
    Public ReadOnly Import As New RoutedUICommand("Import", "Import", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.I, ModifierKeys.Control)}))
    Public ReadOnly NewFile As New RoutedUICommand("NewFile", "NewFile", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.N, ModifierKeys.Control)}))
    Public ReadOnly Open As New RoutedUICommand("Open", "Open", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.O, ModifierKeys.Control)}))
    Public ReadOnly Save As New RoutedUICommand("Save", "Save", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.S, ModifierKeys.Control)}))
    Public ReadOnly PlayPause As New RoutedUICommand("PlayPause", "PlayPause", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.Space, ModifierKeys.None)}))
    Public ReadOnly Draw As New RoutedUICommand("Draw", "Draw", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.Q, ModifierKeys.Control)}))
    Public ReadOnly ColorWhite As New RoutedUICommand("ColorWhite", "ColorWhite", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.W, ModifierKeys.Control)}))
    Public ReadOnly Left As New RoutedUICommand("Left", "Left", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.Left, ModifierKeys.None)}))
    Public ReadOnly Right As New RoutedUICommand("Right", "Right", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.Right, ModifierKeys.None)}))
    Public ReadOnly Up As New RoutedUICommand("Up", "Up", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.Up, ModifierKeys.None)}))
    Public ReadOnly Down As New RoutedUICommand("Down", "Down", GetType(Commands), New InputGestureCollection({New KeyGesture(Key.Down, ModifierKeys.None)}))
End Module
