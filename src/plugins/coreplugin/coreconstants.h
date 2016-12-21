#ifndef CORECONSTANTS_H
#define CORECONSTANTS_H

#include <QtGlobal>

namespace Core {
namespace Constants {

const char FOUNDED_AT[]          = "2016";

// Menubar
const char MENU_BAR[]              = "App.MenuBar";

// Menus
const char M_FILE[]                = "App.Menu.File";
const char M_FILE_RECENTFILES[]    = "App.Menu.File.RecentFiles";
const char M_EDIT[]                = "App.Menu.Edit";
const char M_EDIT_ADVANCED[]       = "App.Menu.Edit.Advanced";
const char M_TOOLS[]               = "App.Menu.Tools";
const char M_TOOLS_EXTERNAL[]      = "App.Menu.Tools.External";
const char M_WINDOW[]              = "App.Menu.Window";
const char M_WINDOW_PANES[]        = "App.Menu.Window.Panes";
const char M_WINDOW_VIEWS[]        = "App.Menu.Window.Views";
const char M_HELP[]                = "App.Menu.Help";

// Contexts
const char C_GLOBAL[]              = "Global Context";

const char C_EDITORMANAGER[]       = "Core.EditorManager";
const char C_NAVIGATION_PANE[]     = "Core.NavigationPane";
const char C_PROBLEM_PANE[]        = "Core.ProblemPane";
const char C_GENERAL_OUTPUT_PANE[] = "Core.GeneralOutputPane";

const char GOTO[]                  = "App.Goto";

const char NEW[]                   = "App.New";
const char OPEN[]                  = "App.Open";
const char SAVE[]                  = "App.Save";
const char PRINT[]                 = "App.Print";
const char EXIT[]                  = "App.Exit";

const char OPTIONS[]               = "App.Options";
const char TOGGLE_SIDEBAR[]        = "App.ToggleSidebar";
const char TOGGLE_MODE_SELECTOR[]  = "App.ToggleModeSelector";
const char TOGGLE_FULLSCREEN[]     = "App.ToggleFullScreen";
const char THEMEOPTIONS[]          = "App.ThemeOptions";

const char TR_SHOW_SIDEBAR[]       = QT_TRANSLATE_NOOP("Core", "Show Sidebar");
const char TR_HIDE_SIDEBAR[]       = QT_TRANSLATE_NOOP("Core", "Hide Sidebar");

const char MINIMIZE_WINDOW[]       = "App.MinimizeWindow";
const char ZOOM_WINDOW[]           = "App.ZoomWindow";
const char CLOSE_WINDOW[]           = "App.CloseWindow";

const char SPLIT[]                 = "App.Split";
const char SPLIT_SIDE_BY_SIDE[]    = "App.SplitSideBySide";
const char SPLIT_NEW_WINDOW[]      = "App.SplitNewWindow";
const char REMOVE_CURRENT_SPLIT[]  = "App.RemoveCurrentSplit";
const char REMOVE_ALL_SPLITS[]     = "App.RemoveAllSplits";
const char GOTO_PREV_SPLIT[]       = "App.GoToPreviousSplit";
const char GOTO_NEXT_SPLIT[]       = "App.GoToNextSplit";
const char CLOSE[]                 = "App.Close";
const char CLOSE_ALTERNATIVE[]     = "App.Close_Alternative"; // temporary, see AppBUG-72
const char CLOSEALL[]              = "App.CloseAll";
const char CLOSEOTHERS[]           = "App.CloseOthers";
const char CLOSEALLEXCEPTVISIBLE[] = "App.CloseAllExceptVisible";
const char GOTONEXT[]              = "App.GotoNext";
const char GOTOPREV[]              = "App.GotoPrevious";
const char GOTONEXTINHISTORY[]     = "App.GotoNextInHistory";
const char GOTOPREVINHISTORY[]     = "App.GotoPreviousInHistory";
const char GO_BACK[]               = "App.GoBack";
const char GO_FORWARD[]            = "App.GoForward";
const char ABOUT_APP[]             = "App.AboutApplication";
const char ABOUT_PLUGINS[]         = "App.AboutPlugins";
const char S_RETURNTOEDITOR[]      = "App.ReturnToEditor";

// Default groups
const char G_DEFAULT_ONE[]         = "App.Group.Default.One";
const char G_DEFAULT_TWO[]         = "App.Group.Default.Two";
const char G_DEFAULT_THREE[]       = "App.Group.Default.Three";

// Main menu bar groups
const char G_FILE[]                = "App.Group.File";
const char G_EDIT[]                = "App.Group.Edit";
const char G_VIEW[]                = "App.Group.View";
const char G_TOOLS[]               = "App.Group.Tools";
const char G_WINDOW[]              = "App.Group.Window";
const char G_HELP[]                = "App.Group.Help";

// File menu groups
const char G_FILE_NEW[]            = "App.Group.File.New";
const char G_FILE_OPEN[]           = "App.Group.File.Open";
const char G_FILE_PROJECT[]        = "App.Group.File.Project";
const char G_FILE_SAVE[]           = "App.Group.File.Save";
const char G_FILE_CLOSE[]          = "App.Group.File.Close";
const char G_FILE_PRINT[]          = "App.Group.File.Print";
const char G_FILE_OTHER[]          = "App.Group.File.Other";

// Edit menu groups
const char G_EDIT_UNDOREDO[]       = "App.Group.Edit.UndoRedo";
const char G_EDIT_COPYPASTE[]      = "App.Group.Edit.CopyPaste";
const char G_EDIT_SELECTALL[]      = "App.Group.Edit.SelectAll";
const char G_EDIT_ADVANCED[]       = "App.Group.Edit.Advanced";

const char G_EDIT_FIND[]           = "App.Group.Edit.Find";
const char G_EDIT_OTHER[]          = "App.Group.Edit.Other";

// Advanced edit menu groups
const char G_EDIT_FORMAT[]         = "App.Group.Edit.Format";
const char G_EDIT_COLLAPSING[]     = "App.Group.Edit.Collapsing";
const char G_EDIT_TEXT[]           = "App.Group.Edit.Text";
const char G_EDIT_BLOCKS[]         = "App.Group.Edit.Blocks";
const char G_EDIT_FONT[]           = "App.Group.Edit.Font";
const char G_EDIT_EDITOR[]         = "App.Group.Edit.Editor";

const char G_TOOLS_OPTIONS[]       = "App.Group.Tools.Options";

// Window menu groups
const char G_WINDOW_SIZE[]         = "App.Group.Window.Size";
const char G_WINDOW_PANES[]        = "App.Group.Window.Panes";
const char G_WINDOW_VIEWS[]        = "App.Group.Window.Views";
const char G_WINDOW_SPLIT[]        = "App.Group.Window.Split";
const char G_WINDOW_NAVIGATE[]     = "App.Group.Window.Navigate";
const char G_WINDOW_LIST[]         = "App.Group.Window.List";
const char G_WINDOW_OTHER[]        = "App.Group.Window.Other";

// Help groups (global)
const char G_HELP_HELP[]           = "App.Group.Help.Help";
const char G_HELP_SUPPORT[]        = "App.Group.Help.Supprt";
const char G_HELP_ABOUT[]          = "App.Group.Help.About";
const char G_HELP_UPDATES[]        = "App.Group.Help.Updates";

const char WIZARD_CATEGORY_QT[] = "R.Qt";
const char WIZARD_TR_CATEGORY_QT[] = QT_TRANSLATE_NOOP("Core", "Qt");
const char WIZARD_KIND_UNKNOWN[] = "unknown";
const char WIZARD_KIND_PROJECT[] = "project";
const char WIZARD_KIND_FILE[] = "file";

const char SETTINGS_CATEGORY_CORE[] = "A.Core";
const char SETTINGS_CATEGORY_CORE_ICON[] = ":/core/images/category_core.png";
const char SETTINGS_TR_CATEGORY_CORE[] = QT_TRANSLATE_NOOP("Core", "Environment");
const char SETTINGS_ID_INTERFACE[] = "A.Interface";
const char SETTINGS_ID_SYSTEM[] = "B.Core.System";
const char SETTINGS_ID_SHORTCUTS[] = "C.Keyboard";
const char SETTINGS_ID_TOOLS[] = "D.ExternalTools";
const char SETTINGS_ID_MIMETYPES[] = "E.MimeTypes";

const char SETTINGS_DEFAULTTEXTENCODING[] = "General/DefaultFileEncoding";

const char SETTINGS_THEME[] = "Core/AppTheme";
const char DEFAULT_THEME[] = "flat";

const char ALL_FILES_FILTER[]      = QT_TRANSLATE_NOOP("Core", "All Files (*)");

const char TR_CLEAR_MENU[]         = QT_TRANSLATE_NOOP("Core", "Clear Menu");

const int TARGET_ICON_SIZE = 32;

} // namespace Constants
} // namespace Core

#endif // CORECONSTANTS_H
