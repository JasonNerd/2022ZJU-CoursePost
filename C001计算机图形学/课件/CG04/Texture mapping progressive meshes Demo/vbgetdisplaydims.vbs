'********************************************************************
'*
'* File:           ListDisplayConfig.vbs 
'* Created:        March 1999
'* Version:        1.0
'*
'*  Main Function:  Obtains the display configuration of a machine.
'*
'*  ListDisplayConfig.vbs  [/S <server>] [/U <username>] [/W <password>] 
'*                         [/O <outputfile>]
'*
'* Copyright (C) 1999 Microsoft Corporation
'*
'********************************************************************

OPTION EXPLICIT

    'Declare variables
    Dim strServer, strUserName, strPassword, strOutputFile

    'Make sure the host is csript, if not then abort
    VerifyHostIsCscript()

    Call GetDispInfo(strServer     , _
		     strOutputFile , _
		     strUserName   , _
		     strPassword     )

'********************************************************************
'* End of Script
'********************************************************************

'********************************************************************
'*
'* Sub GetDispInfo()
'* Purpose: Obtains the display configuration of a machine.
'* Input:   strServer           a machine name
'*          strOutputFile       an output file name
'*          strUserName         the current user's name
'*          strPassword         the current user's password
'*
'* Output:  Results are either printed on screen or saved in strOutputFile.
'*
'********************************************************************
Private Sub GetDispInfo(strServer     , _
                        strOutputFile , _
                        strUserName   , _
                        strPassword     )

    ON ERROR RESUME NEXT

    Dim objFileSystem, objOutputFile, objService, objWshNet
    Dim objInst,  objDisSet, objDis
    Dim strQuery, strMessage, strCat
    Dim intGetFirst

    Dim oneLiner

    'Wscript.echo("count=" & Wscript.Arguments.Count)
    oneLiner = Wscript.Arguments.Count > 0
    'If Wscript.Arguments.Count > 0 Then
    '    strFlag = Wscript.arguments.Item(0)
    'End If
    'If IsEmpty(strFlag) Then                'No arguments have been received
    '    Exit Function
    'End If

    'Establish a connection with the server.
    If blnConnect("root\cimv2" , _
                   strUserName , _
                   strPassword , _
                   strServer   , _
                   objService  ) Then
        Call Wscript.Echo("")
        Call Wscript.Echo("Please check the server name, " _
                        & "credentials and WBEM Core.")
        Exit Sub
    End If

    'Get the display configuration
    Set objDisSet = objService.InstancesOf("Win32_DisplayConfiguration")
    If Err.Number Then
        Wscript.Echo("Error 0x" & CStr(Hex(Err.Number)) _
                   & " occurred getting the memory configuration.")
        If Err.Description <> "" Then
            Wscript.Echo("Error description: " & Err.Description & ".")
        End If
        Err.Clear
        Exit Sub
    End If

    intGetFirst = 0

    For Each objInst in objDisSet
        If intGetFirst = 0 Then Set objDis = objInst
            intGetFirst = intGetFirst + 1
        Next

    If IsEmpty(strServer) Then
        Set objWshNet = CreateObject("Wscript.Network")
            strServer = objWshNet.ComputerName
    End If

    if oneLiner then
      Call WriteLine(objDis.PelsWidth & " " & objDis.PelsHeight & " " & _
        objDis.BitsPerPel, objOutputFile)
    else
      'Call WriteLine("Display Configuration for Machine " & _
      '                strServer, objOutputFile)
      'Call WriteLine("Device Name       = " & objDis.DeviceName, _
      '                objOutputFile)
      'Call WriteLine("Driver Version    = " & objDis.DriverVersion, _
      '                objOutputFile)
      'Call WriteLine("Display Frequency = " & objDis.DisplayFrequency, _
      '                objOutputFile)
      Call WriteLine("DISPLAYWIDTH=" & objDis.PelsWidth, _
		      objOutputFile)
      Call WriteLine("DISPLAYHEIGHT=" & objDis.PelsHeight, _
		      objOutputFile)
      Call WriteLine("DISPLAYDEPTH=" & objDis.BitsPerPel, _
		      objOutputFile)
    end if
    
    If IsObject(objOutputFile) Then
        objOutputFile.Close
        Call Wscript.Echo ("Results are saved in file " & strOutputFile & ".")
    End If

End Sub

'********************************************************************
'*
'* Function blnConnect()
'*
'* Purpose: Connects to machine strServer.
'*
'* Input:   strServer       a machine name
'*          strNameSpace    a namespace
'*          strUserName     name of the current user
'*          strPassword     password of the current user
'*
'* Output:  objService is returned  as a service object.
'*          strServer is set to local host if left unspecified
'*
'********************************************************************
Private Function blnConnect(ByVal strNameSpace, _
                            ByVal strUserName,  _
                            ByVal strPassword,  _
                            ByRef strServer,    _
                            ByRef objService)

    ON ERROR RESUME NEXT

    Dim objLocator, objWshNet

    blnConnect = False     'There is no error.

    'Create Locator object to connect to remote CIM object manager
    Set objLocator = CreateObject("WbemScripting.SWbemLocator")
    If Err.Number then
        Call Wscript.Echo( "Error 0x" & CStr(Hex(Err.Number)) & _
                           " occurred in creating a locator object." )
        If Err.Description <> "" Then
            Call Wscript.Echo( "Error description: " & Err.Description & "." )
        End If
        Err.Clear
        blnConnect = True     'An error occurred
        Exit Function
    End If

    'Connect to the namespace which is either local or remote
    Set objService = objLocator.ConnectServer (strServer, strNameSpace, _
       strUserName, strPassword)
    ObjService.Security_.impersonationlevel = 3
    If Err.Number then
        Call Wscript.Echo( "Error 0x" & CStr(Hex(Err.Number)) & _
                           " occurred in connecting to server " _
           & strServer & ".")
        If Err.Description <> "" Then
            Call Wscript.Echo( "Error description: " & Err.Description & "." )
        End If
        Err.Clear
        blnConnect = True     'An error occurred
    End If

    'Get the current server's name if left unspecified
    If IsEmpty(strServer) Then
        Set objWshNet = CreateObject("Wscript.Network")
    strServer     = objWshNet.ComputerName
    End If

End Function

'********************************************************************
'*
'* Sub      VerifyHostIsCscript()
'*
'* Purpose: Determines which program is used to run this script.
'*
'* Input:   None
'*
'* Output:  If host is not cscript, then an error message is printed 
'*          and the script is aborted.
'*
'********************************************************************
Sub VerifyHostIsCscript()

    CONST CONST_ERROR                   = 0
    CONST CONST_WSCRIPT                 = 1
    CONST CONST_CSCRIPT                 = 2
    ON ERROR RESUME NEXT

    Dim strFullName, strCommand, i, j, intStatus

    strFullName = WScript.FullName

    If Err.Number then
        Call Wscript.Echo( "Error 0x" & CStr(Hex(Err.Number)) & " occurred." )
        If Err.Description <> "" Then
            Call Wscript.Echo( "Error description: " & Err.Description & "." )
        End If
        intStatus =  CONST_ERROR
    End If

    i = InStr(1, strFullName, ".exe", 1)
    If i = 0 Then
        intStatus =  CONST_ERROR
    Else
        j = InStrRev(strFullName, "\", i, 1)
        If j = 0 Then
            intStatus =  CONST_ERROR
        Else
            strCommand = Mid(strFullName, j+1, i-j-1)
            Select Case LCase(strCommand)
                Case "cscript"
                    intStatus = CONST_CSCRIPT
                Case "wscript"
                    intStatus = CONST_WSCRIPT
                Case Else       'should never happen
                    Call Wscript.Echo( "An unexpected program was used to " _
                                       & "run this script." )
                    Call Wscript.Echo( "Only CScript.Exe or WScript.Exe can " _
                                       & "be used to run this script." )
                    intStatus = CONST_ERROR
                End Select
        End If
    End If

    If intStatus <> CONST_CSCRIPT Then
        Call WScript.Echo( "Please run this script using CScript." & vbCRLF & _
             "This can be achieved by" & vbCRLF & _
             "1. Using ""CScript ListDisplayConfig.vbs arguments"" for Windows 95/98 or" _
             & vbCRLF & "2. Changing the default Windows Scripting Host " _
             & "setting to CScript" & vbCRLF & "    using ""CScript " _
             & "//H:CScript //S"" and running the script using" & vbCRLF & _
             "    ""ListDisplayConfig.vbs arguments"" for Windows NT/2000." )
        WScript.Quit
    End If

End Sub

'********************************************************************
'*
'* Sub WriteLine()
'* Purpose: Writes a text line either to a file or on screen.
'* Input:   strMessage  the string to print
'*          objFile     an output file object
'* Output:  strMessage is either displayed on screen or written to a file.
'*
'********************************************************************
Sub WriteLine(ByVal strMessage, ByVal objFile)

    On Error Resume Next
    If IsObject(objFile) then        'objFile should be a file object
        objFile.WriteLine strMessage
    Else
        Call Wscript.Echo( strMessage )
    End If

End Sub

'********************************************************************
'* 
'* Function blnErrorOccurred()
'*
'* Purpose: Reports error with a string saying what the error occurred in.
'*
'* Input:   strIn		string saying what the error occurred in.
'*
'* Output:  displayed on screen 
'* 
'********************************************************************
Private Function blnErrorOccurred (ByVal strIn)

    If Err.Number Then
        Call Wscript.Echo( "Error 0x" & CStr(Hex(Err.Number)) & ": " & strIn)
        If Err.Description <> "" Then
            Call Wscript.Echo( "Error description: " & Err.Description)
        End If
        Err.Clear
        blnErrorOccurred = True
    Else
        blnErrorOccurred = False
    End If

End Function
