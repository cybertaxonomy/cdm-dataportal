function popupExternalLinks( x_url)
{
	var		oWindow = null;
	var		iWidth = 1050;
	var		iHeight = 700;
	oWindow = window.open( x_url, "POPUP_EXTERNAL_LINKS", "dependent=yes,locationbar=yes,menubar=yes,scrollbars=yes,resizable=yes,toolbar=yes,status=no,screenX=0,screenY=0,height="+iHeight+",width="+iWidth); 
	if( top.window.opener)
	oWindow.opener = top.window.opener;
	oWindow.focus();
}
