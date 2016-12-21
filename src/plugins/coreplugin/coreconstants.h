#ifndef CORECONSTANTS_H
#define CORECONSTANTS_H

#include <QtGlobal>

namespace Core {
namespace Constants {

const char FOUNDED_AT[]          = "2015";

// Menubar
const char MENU_BAR[]              = "Athletic.MenuBar";

// Menus
const char M_FILE[]                = "Athletic.Menu.File";
const char M_FILE_RECENTFILES[]    = "Athletic.Menu.File.RecentFiles";
const char M_EDIT[]                = "Athletic.Menu.Edit";
const char M_EDIT_ADVANCED[]       = "Athletic.Menu.Edit.Advanced";
const char M_TOOLS[]               = "Athletic.Menu.Tools";
const char M_TOOLS_EXTERNAL[]      = "Athletic.Menu.Tools.External";
const char M_WINDOW[]              = "Athletic.Menu.Window";
const char M_WINDOW_PANES[]        = "Athletic.Menu.Window.Panes";
const char M_WINDOW_VIEWS[]        = "Athletic.Menu.Window.Views";
const char M_HELP[]                = "Athletic.Menu.Help";

// Contexts
const char C_GLOBAL[]              = "Global Context";

const char C_EDITORMANAGER[]       = "Core.EditorManager";
const char C_NAVIGATION_PANE[]     = "Core.NavigationPane";
const char C_PROBLEM_PANE[]        = "Core.ProblemPane";
const char C_GENERAL_OUTPUT_PANE[] = "Core.GeneralOutputPane";

const char GOTO[]                  = "Athletic.Goto";

const char NEW[]                   = "Athletic.New";
const char OPEN[]                  = "Athletic.Open";
const char SAVE[]                  = "Athletic.Save";
const char PRINT[]                 = "Athletic.Print";
const char EXIT[]                  = "Athletic.Exit";

const char OPTIONS[]               = "Athletic.Options";
const char TOGGLE_SIDEBAR[]        = "Athletic.ToggleSidebar";
const char TOGGLE_MODE_SELECTOR[]  = "Athletic.ToggleModeSelector";
const char TOGGLE_FULLSCREEN[]     = "Athletic.ToggleFullScreen";
const char THEMEOPTIONS[]          = "Athletic.ThemeOptions";

const char TR_SHOW_SIDEBAR[]       = QT_TRANSLATE_NOOP("Core", "Show Sidebar");
const char TR_HIDE_SIDEBAR[]       = QT_TRANSLATE_NOOP("Core", "Hide Sidebar");

const char MINIMIZE_WINDOW[]       = "Athletic.MinimizeWindow";
const char ZOOM_WINDOW[]           = "Athletic.ZoomWindow";
const char CLOSE_WINDOW[]           = "Athletic.CloseWindow";

const char SPLIT[]                 = "Athletic.Split";
const char SPLIT_SIDE_BY_SIDE[]    = "Athletic.SplitSideBySide";
const char SPLIT_NEW_WINDOW[]      = "Athletic.SplitNewWindow";
const char REMOVE_CURRENT_SPLIT[]  = "Athletic.RemoveCurrentSplit";
const char REMOVE_ALL_SPLITS[]     = "Athletic.RemoveAllSplits";
const char GOTO_PREV_SPLIT[]       = "Athletic.GoToPreviousSplit";
const char GOTO_NEXT_SPLIT[]       = "Athletic.GoToNextSplit";
const char CLOSE[]                 = "Athletic.Close";
const char CLOSE_ALTERNATIVE[]     = "Athletic.Close_Alternative"; // temporary, see AthleticBUG-72
const char CLOSEALL[]              = "Athletic.CloseAll";
const char CLOSEOTHERS[]           = "Athletic.CloseOthers";
const char CLOSEALLEXCEPTVISIBLE[] = "Athletic.CloseAllExceptVisible";
const char GOTONEXT[]              = "Athletic.GotoNext";
const char GOTOPREV[]              = "Athletic.GotoPrevious";
const char GOTONEXTINHISTORY[]     = "Athletic.GotoNextInHistory";
const char GOTOPREVINHISTORY[]     = "Athletic.GotoPreviousInHistory";
const char GO_BACK[]               = "Athletic.GoBack";
const char GO_FORWARD[]            = "Athletic.GoForward";
const char ABOUT_ATHLETIC[]        = "Athletic.AboutAthletic";
const char ABOUT_PLUGINS[]         = "Athletic.AboutPlugins";
const char S_RETURNTOEDITOR[]      = "Athletic.ReturnToEditor";

// Default groups
const char G_DEFAULT_ONE[]         = "Athletic.Group.Default.One";
const char G_DEFAULT_TWO[]         = "Athletic.Group.Default.Two";
const char G_DEFAULT_THREE[]       = "Athletic.Group.Default.Three";

// Main menu bar groups
const char G_FILE[]                = "Athletic.Group.File";
const char G_EDIT[]                = "Athletic.Group.Edit";
const char G_VIEW[]                = "Athletic.Group.View";
const char G_TOOLS[]               = "Athletic.Group.Tools";
const char G_WINDOW[]              = "Athletic.Group.Window";
const char G_HELP[]                = "Athletic.Group.Help";

// File menu groups
const char G_FILE_NEW[]            = "Athletic.Group.File.New";
const char G_FILE_OPEN[]           = "Athletic.Group.File.Open";
const char G_FILE_PROJECT[]        = "Athletic.Group.File.Project";
const char G_FILE_SAVE[]           = "Athletic.Group.File.Save";
const char G_FILE_CLOSE[]          = "Athletic.Group.File.Close";
const char G_FILE_PRINT[]          = "Athletic.Group.File.Print";
const char G_FILE_OTHER[]          = "Athletic.Group.File.Other";

// Edit menu groups
const char G_EDIT_UNDOREDO[]       = "Athletic.Group.Edit.UndoRedo";
const char G_EDIT_COPYPASTE[]      = "Athletic.Group.Edit.CopyPaste";
const char G_EDIT_SELECTALL[]      = "Athletic.Group.Edit.SelectAll";
const char G_EDIT_ADVANCED[]       = "Athletic.Group.Edit.Advanced";

const char G_EDIT_FIND[]           = "Athletic.Group.Edit.Find";
const char G_EDIT_OTHER[]          = "Athletic.Group.Edit.Other";

// Advanced edit menu groups
const char G_EDIT_FORMAT[]         = "Athletic.Group.Edit.Format";
const char G_EDIT_COLLAPSING[]     = "Athletic.Group.Edit.Collapsing";
const char G_EDIT_TEXT[]           = "Athletic.Group.Edit.Text";
const char G_EDIT_BLOCKS[]         = "Athletic.Group.Edit.Blocks";
const char G_EDIT_FONT[]           = "Athletic.Group.Edit.Font";
const char G_EDIT_EDITOR[]         = "Athletic.Group.Edit.Editor";

const char G_TOOLS_OPTIONS[]       = "Athletic.Group.Tools.Options";

// Window menu groups
const char G_WINDOW_SIZE[]         = "Athletic.Group.Window.Size";
const char G_WINDOW_PANES[]        = "Athletic.Group.Window.Panes";
const char G_WINDOW_VIEWS[]        = "Athletic.Group.Window.Views";
const char G_WINDOW_SPLIT[]        = "Athletic.Group.Window.Split";
const char G_WINDOW_NAVIGATE[]     = "Athletic.Group.Window.Navigate";
const char G_WINDOW_LIST[]         = "Athletic.Group.Window.List";
const char G_WINDOW_OTHER[]        = "Athletic.Group.Window.Other";

// Help groups (global)
const char G_HELP_HELP[]           = "Athletic.Group.Help.Help";
const char G_HELP_SUPPORT[]        = "Athletic.Group.Help.Supprt";
const char G_HELP_ABOUT[]          = "Athletic.Group.Help.About";
const char G_HELP_UPDATES[]        = "Athletic.Group.Help.Updates";

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

const char SETTINGS_THEME[] = "Core/AthleticTheme";
const char DEFAULT_THEME[] = "flat";

const char ALL_FILES_FILTER[]      = QT_TRANSLATE_NOOP("Core", "All Files (*)");

const char TR_CLEAR_MENU[]         = QT_TRANSLATE_NOOP("Core", "Clear Menu");

const int TARGET_ICON_SIZE = 32;

} // namespace Constants
} // namespace Core

#endif // CORECONSTANTS_H
