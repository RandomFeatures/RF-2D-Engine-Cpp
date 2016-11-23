#include "RF3d_Global.h"


// Local includes
#include "WinMain.h"



void cApp::ConsoleFrame(void *Ptr, long Purpose)
{
///*****Local Variables Start*****/
//  cApp *g_Game = (cApp*)Ptr;
//  static cBaseGUIWindow ConsoleWindow;   // Window for debuging
//  static cFont          ConsoleFont;	
//
///*****Local Variables End*****/
//
//  // Initialize console window
//  if(Purpose == INITPURPOSE) 
//  {
//	  ConsoleFont.Create(&g_Game->m_Graphics, "MS Sans Serif", 8, TRUE);
//
//  	  ConsoleWindow.Create(&g_Game->m_Graphics, "gui\\Border.bmp", g_Game->m_BasePath.c_str());
//	  
//   	  ConsoleWindow.Move(0,0,g_Game->m_Width,g_Game->m_Height,D3DCOLOR_RGBA(0,64,128,255));
//
//   	  ConsoleWindow.m_Visible = true;
//	  return;
//  }
//
//  // Shutdown console window
//  if(Purpose == SHUTDOWNPURPOSE) 
//  {
//	  ConsoleFont.Free();
//	  ConsoleWindow.Free();
//      return;
//  }
//
//  if (!g_Game->CaptureConsoleInput())
//	  return;
//
//
//  // Render the frame's graphics
//  if(g_Game->m_Graphics.BeginScene() == TRUE) 
//  {
//
//	  ConsoleWindow.Render("", false);
//
//      // display all previous commands;
//	  //TODO -- Dont write off the top of the screen
//	  char  tmpstr[256];
//	  //for (int iLoop = g_Game->m_ConsoleList.Length(); iLoop > -1; iLoop --)
//	  //{
//		 // stdStrToChar(g_Game->m_ConsoleList[iLoop],tmpstr);
//		 // ConsoleFont.Print(tmpstr, 10,(g_Game->m_Height - 36)-((g_Game->m_ConsoleList.Length() - iLoop) *13));
//	  //}
//	  //format the command prompt before displaying it	
//	  strcpy(tmpstr, ">");	
//	  strcat(tmpstr, CurrentText.c_str());
//	  strcat(tmpstr, "_");
//	  ConsoleFont.Print(tmpstr, 10, g_Game->m_Height - 24);
//
//	//blit cursor
//      g_Game->m_Cursor.Blit(g_Game->m_Mouse.GetXPos(),g_Game->m_Mouse.GetYPos());
//    
//	  g_Game->m_Graphics.EndScene();
//  }
//  g_Game->m_Graphics.Display();
//
}

BOOL cApp::CaptureConsoleInput()
{
//  static std::string    PreviousText;
//  // Ignore the ALT key
//  if(m_Keyboard.GetKeyState(KEY_ALT) == TRUE)  
//  {
//    return FALSE;
//  }
//  
//  //send the current command to the parser
//  if(m_Keyboard.GetKeyState(KEY_ENTER) == TRUE) 
//  {
//    m_Keyboard.SetLock(KEY_ENTER);
//	if (!CurrentText.empty())
//	{ 
//		PreviousText = CurrentText;
//		char tmpStr[1024];
//		strcpy(tmpStr,CurrentText.c_str());
//		//pass it on to the script parser
//		//if (!m_PyScript.RunText(tmpStr))
//		//{
//		//	CurrentText = "Error: " + CurrentText + "\n";
//		//}
//		//else
//   		CurrentText = CurrentText + "\n";
//
////		m_ConsoleList.Add(CurrentText);
//		CurrentText = "";
//
//	}
//	else DisplayLogInConsole();
//	
//  }
//
//  
//  if(m_Keyboard.GetKeyState(KEY_BACKSPACE) == TRUE) 
//  {
//    m_Keyboard.SetLock(KEY_BACKSPACE);
//	if (!CurrentText.empty())
//	{ //trim off the last character
//		char tstr[256];
//		strcpy(tstr, CurrentText.c_str());
//		tstr[strlen(tstr)-1] = 0;
//		CurrentText = tstr;
//	}
//	
//  }
//
//  if(m_Keyboard.GetKeyState(KEY_TAB) == TRUE) 
//  {
//    m_Keyboard.SetLock(KEY_TAB);
//	CurrentText = PreviousText;
//	
//  }
//
///*
//  if(m_Keyboard.GetKeyState(KEY_SPACE) == TRUE) 
//  {
//    m_Keyboard.SetLock(KEY_SPACE);
//	FILE *fp = fopen("D:\\RF3DDev\\Resources\\Scripts\\test.py", "r");
//	PyRun_SimpleFile(fp,"D:\\RF3DDev\\Resources\\Scripts\\test.py");
//	fclose(fp);
//	if(PyErr_Occurred())
//	{CurrentText = CurrentText + "No\n";}
//	else
//	{CurrentText = CurrentText + "Yes\n";}
//
//	m_ConsoleList.Add(CurrentText);
//	CurrentText = "";
//
//  }
//*/
//  // Close the console window
//  if(m_Keyboard.GetKeyState(KEY_TILDE) == TRUE) 
//  {
//    m_Keyboard.SetLock(KEY_TILDE);
//	m_Graphics.EnableLighting(m_LevelFileData.UseLighting);
//    // Pop all states
//	m_StateManager.PopAll(this);
//	m_StateManager.Push(MainGameFrame, this);
//    return FALSE;
//  }
//  
//  // Close the console window
//  if(m_Keyboard.GetKeyState(KEY_ESC) == TRUE) 
//  {
//    m_Keyboard.SetLock(KEY_ESC);
//	m_Graphics.EnableLighting(m_LevelFileData.UseLighting);
//    // Pop all states
//	m_StateManager.Pop(this);
//	m_StateManager.Push(MainGameFrame, this);
//    return FALSE;
//  }
//
//  short result = m_Keyboard.GetCurrentKeypress();
//
//  //turn the keypress in to something readable
//  if (result > 0 )
//  {
//     CurrentText = CurrentText + char(result);
//  }
//	
  return TRUE;
}

void cApp::DisplayLogInConsole()
{
//  char  tmpstr[256];
  //for (int iLoop = 0; iLoop < (m_logfile.m_strList.Length()); iLoop ++)
  //{
	 //stdStrToChar(m_logfile.m_strList[iLoop],tmpstr);	
  //   m_ConsoleList.Add(tmpstr);
  //}


}
