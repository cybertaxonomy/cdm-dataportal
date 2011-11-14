function popupExternalLinks( x_url)
{
	var		oWindow = null;
	var		iWidth = 820;
	var		iHeight = 700;
	oWindow = window.open( x_url, "POPUP_EXTERNAL_LINKS", "dependent=yes,locationbar=no,menubar=no,scrollbars=yes,resizable=yes,status=no,screenX=0,screenY=0,height="+iHeight+",width="+iWidth); 
	if( top.window.opener)
	oWindow.opener = top.window.opener;
	oWindow.focus();
}
