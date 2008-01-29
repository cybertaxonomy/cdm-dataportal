/* $Id: jlightbox.uncompressed.js,v 1.1.2.2 2007/09/19 00:34:00 sun Exp $ */
/**
 *************************************************************************
 * This is a slightly modified version of the original jQuery Lightbox:
 *  - the order of imageDataContainer and outerImageContainer has been reverted
 *  - fileLoadingImage, fileBottomNavCloseImage are replaced by css background images
 * a.kohlbecker 2008
 *************************************************************************
 * @author
 *   Daniel F. Kudwien (sun), <http://www.unleashedmind.com>
 *   Warren Krewenki, <http://warren.mesozen.com>
 *
 * Based on Lightbox v2.03.3 by Lokesh Dhakar
 * <http://www.huddletogether.com/projects/lightbox2>
 *
 * Originally written using the Prototype framework and Script.aculo.us, now
 * re-written using jQuery. This version tries to stay as much comparable to the
 * original script as possible. There will be another, experimental edition of
 * jQuery Lightbox leveraging the complete jQuery framework.
 */
var Lightbox = {
	//fileLoadingImage: '/sites/all/modules/jlightbox/images/loading.gif',
	//fileBottomNavCloseImage: '/sites/all/modules/jlightbox/images/closelabel.gif',
	overlayOpacity: 0.4,
	resizeSpeed: 'normal',
	borderSize: 10,
	imageArray: new Array,
	activeImage: 0,
	overlaySpeed: 'slow', // shadow fade in/out duration
	
	initialize: function() {	
		
		Lightbox.updateImageList();
		
		// Attribute galleryimg="false" hides IE image toolbar.
		$("body").append('<div id="overlay"></div> \
			<div id="lightbox"> \
			<div id="imageDataContainer"> \
          <div id="imageData"> \
            <div id="imageDetails"> \
              <span id="caption"></span> \
              <span id="numberDisplay"></span> \
            </div> \
            <div id="bottomNav"> \
              <a href="#" id="bottomNavClose"> \
                <img src="'+ Lightbox.fileBottomNavCloseImage +'" /> \
              </a> \
            </div> \
          </div> \
        </div> \
				<div id="outerImageContainer"> \
					<div id="imageContainer"> \
						<img id="lightboxImage" galleryimg="false" /> \
						<div style="" id="hoverNav"> \
							<a href="#" id="prevLink"></a> \
							<a href="#" id="nextLink"></a> \
						</div> \
						<div id="loading"> \
							<a href="#" id="loadingLink"> \
							</a> \
						</div> \
					</div> \
				</div> \
			</div>\
			');
		$('#overlay').click(function(){ Lightbox.end(); return false; }).hide();
		$('#lightbox').hide();
		$('#loadingLink').click(function(){ Lightbox.end(); return false; });
		$('#bottomNavClose').click(function(){ Lightbox.end(); return false; });
		$('#outerImageContainer').width(250).height(250);

		// Add padding for navigation links. 18/09/2007 sun
		$('#prevLink').css({ paddingTop: Lightbox.borderSize, paddingLeft: Lightbox.borderSize });
		$('#nextLink').css({ paddingTop: Lightbox.borderSize, paddingRight: Lightbox.borderSize });
		
		// Setup onclick handlers for previous and next buttons ONCE.
		// Lightbox wacks out if we reset those in updateNav(). 13/09/2007 sun
		$('#prevLink').click(function() {
				Lightbox.changeImage(Lightbox.activeImage - 1); return false;
		});
		$('#nextLink').click(function() {
				Lightbox.changeImage(Lightbox.activeImage + 1); return false;
		});
	},
	
	//
	// updateImageList()
	// Loops through anchor tags looking for 'lightbox' references and applies onclick
	// events to appropriate links. You can rerun after dynamically adding images w/ajax.
	//
	updateImageList: function() {	
		// attach lightbox to any links with rel 'lightbox'
		var anchors = $('a');
		var areas = $('area');

		// loop through all anchor tags
		for (var i=0; i<anchors.length; i++){
			var anchor = anchors[i];
			
			var relAttribute = String(anchor.getAttribute('rel'));
			
			// use the string.match() method to catch 'lightbox' references in the rel attribute
			if (anchor.getAttribute('href') && (relAttribute.toLowerCase().match('lightbox'))){
				anchor.onclick = function() { Lightbox.start(this); return false; };
			}
		}

		// loop through all area tags
		// todo: combine anchor & area tag loops
		for (var i=0; i< areas.length; i++){
			var area = areas[i];
			
			var relAttribute = String(area.getAttribute('rel'));
			
			// use the string.match() method to catch 'lightbox' references in the rel attribute
			if (area.getAttribute('href') && (relAttribute.toLowerCase().match('lightbox'))){
				area.onclick = function() { Lightbox.start(this); return false; };
			}
		}
	},
	
	//
	//	start()
	//	Display overlay and lightbox. If image is part of a set, add siblings to Lightbox.imageArray.
	//
	start: function(imageLink) {	
		$("select, embed, object").hide();
		
		// stretch overlay to fill page and fade in
		var arrayPageSize = Lightbox.getPageSize();
		// alert(arrayPageSize[0] +' ?= '+ $('body').width());
		// alert(arrayPageSize[1] +' ?= '+ $('body').height());
		$('#overlay').width(arrayPageSize[0]).height(arrayPageSize[1]);

		$('#overlay').css({opacity : Lightbox.overlayOpacity}).fadeIn(Lightbox.overlaySpeed);

		Lightbox.imageArray = new Array;
		imageNum = 0;		

		var anchors = $(imageLink.tagName);
		imageLink = $(imageLink);
	
		// if image is NOT part of a set..
		if((imageLink.attr('rel') == 'lightbox')){
			// add single image to Lightbox.imageArray
			Lightbox.imageArray.push(new Array(imageLink.attr('href'), imageLink.attr('title')));			
		}
		// if image is part of a set..
		else {
			// loop through anchors, find other images in set, and add them to Lightbox.imageArray
			for (var i=0; i < anchors.length; i++){
				var anchor = $(anchors[i]);
				if (anchor.attr('href') && (anchor.attr('rel') == imageLink.attr('rel'))){
					Lightbox.imageArray.push(new Array(anchor.attr('href'), anchor.attr('title')));
				}
			}
			// remove duplicates
			// was: Array.prototype.removeDuplicates()
			for (i=0; i < Lightbox.imageArray.length; i++){
				for(var j = Lightbox.imageArray.length-1; j>i; j--){        
					if(Lightbox.imageArray[i][0] == Lightbox.imageArray[j][0]){
						Lightbox.imageArray.splice(j,1);
					}
				}
			}
			// determine number of clicked image
			while (Lightbox.imageArray[imageNum][0] != imageLink.attr('href')) { imageNum++;}
		}

		// calculate top and left offset for the lightbox 
		var arrayPageScroll = Lightbox.getPageScroll();
		var lightboxTop = arrayPageScroll[1] + (arrayPageSize[3] / 10);
		var lightboxLeft = arrayPageScroll[0];
		$("#lightbox").css({top: lightboxTop, left: lightboxLeft});
		
		$("#lightbox").show();
		
		Lightbox.changeImage(imageNum);
	},

	//
	//	changeImage()
	//	Hide most elements and preload image in preparation for resizing image container.
	//
	changeImage: function(imageNum) {	
		// update global var
		Lightbox.activeImage = imageNum;

		// hide elements during transition
		$('#loading').show();
		$('#lightboxImage, #hoverNav, #imageDataContainer, #numberDisplay').hide();
		
		imgPreloader = new Image();
		
		// once image is preloaded, resize image container
		imgPreloader.onload=function(){
			$('#lightboxImage').attr('src', Lightbox.imageArray[Lightbox.activeImage][0]);
			Lightbox.resizeImageContainer(this.width, this.height);
			
			// clear onLoad, IE behaves irratically with animated gifs otherwise 
			if($.browser.msie) imgPreloader.onload=function(){};
		};
		imgPreloader.src = Lightbox.imageArray[Lightbox.activeImage][0];
	},

	//
	//	resizeImageContainer()
	//
	resizeImageContainer: function(imgWidth, imgHeight) {
		
		// get current width and height
		this.widthCurrent = $('#outerImageContainer').width();
		this.heightCurrent = $('#outerImageContainer').height();

		// get new width and height
		this.widthNew = (imgWidth + (Lightbox.borderSize * 2));
		this.heightNew = (imgHeight + (Lightbox.borderSize * 2));

		// calculate size difference between new and old image, and resize if necessary
		this.wDiff = this.widthCurrent - this.widthNew;
		this.hDiff = this.heightCurrent - this.heightNew;

		if(this.hDiff != 0 || this.wDiff != 0){
			$('#outerImageContainer').animate({width: this.widthNew, height: this.heightNew}, Lightbox.resizeSpeed, 'linear', function() {
				Lightbox.showImage();
			});
		}
		else {
			// if new and old image are same size and no scaling transition is necessary, 
			// do a quick pause to prevent image flicker.
			if ($.browser.msie){ Lightbox.pause(250); } else { Lightbox.pause(100);} 
			Lightbox.showImage();
		}

		$('#prevLink, #nextLink').height(imgHeight);
		$('#prevLink, #nextLink').width(parseInt(imgWidth / 2));
		$('#imageDataContainer').width(this.widthNew);
	},
	
	//
	//	showImage()
	//	Display image and begin preloading neighbors.
	//
	showImage: function(){
		$('#loading').hide();
		$('#lightboxImage').fadeIn(Lightbox.resizeSpeed, Lightbox.updateDetails);
		// Moved preloadNeighborImages() to boost rendering.
	},

	//
	//	updateDetails()
	//	Display caption, image number, and bottom nav.
	//
	updateDetails: function() {
		// if caption is not null
		if(Lightbox.imageArray[Lightbox.activeImage][1]){
			$('#caption').html(Lightbox.imageArray[Lightbox.activeImage][1]).show();
		}
		
		// if image is part of set display 'Image x of x' 
		if(Lightbox.imageArray.length > 1){
			$('#numberDisplay').html("Image " + eval(parseInt(Lightbox.activeImage) + 1) + " of " + Lightbox.imageArray.length).show();
		}

		$("#imageDataContainer").slideDown(Lightbox.resizeSpeed, function() {
			// Usability optimization: Display image navigation first.
			$('#hoverNav').show();				
			Lightbox.updateNav();

			// update overlay size and update nav
			var arrayPageSize = Lightbox.getPageSize();
			$('#overlay').height(arrayPageSize[1]);

			Lightbox.preloadNeighborImages();
			Lightbox.enableKeyboardNav();
		});
	},

	//
	//	updateNav()
	//	Display appropriate previous and next hover navigation.
	//
	updateNav: function() {
		// Since we are working with global variables, onclick handlers are only
		// setup once in initialize(). 13/09/2007 sun

		// if not first image in set, display prev image button
		if(Lightbox.activeImage != 0) {
			$('#prevLink').show();
		}
		else {
			$('#prevLink').hide();
		}
		// if not last image in set, display next image button
		if(Lightbox.activeImage != (Lightbox.imageArray.length - 1)) {
			$('#nextLink').show();
		}
		else {
			$('#nextLink').hide();
		}
	},

	//
	//	enableKeyboardNav()
	//
	enableKeyboardNav: function() {
		document.onkeydown = Lightbox.keyboardAction; 
	},

	//
	//	disableKeyboardNav()
	//
	disableKeyboardNav: function() {
		document.onkeydown = '';
	},

	//
	//	keyboardAction()
	//
	keyboardAction: function(e) {
		if (e == null) { // ie
			keycode = event.keyCode;
			escapeKey = 27;
		} else { // mozilla
			keycode = e.keyCode;
			escapeKey = e.DOM_VK_ESCAPE;
		}

		key = String.fromCharCode(keycode).toLowerCase();
		
		if((key == 'x') || (key == 'o') || (key == 'c') || (keycode == escapeKey)){	// close lightbox
			Lightbox.end();
		} else if((key == 'p') || (keycode == 37)){	// display previous image
			if(Lightbox.activeImage != 0){
				Lightbox.disableKeyboardNav();
				Lightbox.changeImage(Lightbox.activeImage - 1);
			}
		} else if((key == 'n') || (keycode == 39)){	// display next image
			if(Lightbox.activeImage != (Lightbox.imageArray.length - 1)){
				Lightbox.disableKeyboardNav();
				Lightbox.changeImage(Lightbox.activeImage + 1);
			}
		}

	},

	//
	//	preloadNeighborImages()
	//	Preload previous and next images.
	//
	preloadNeighborImages: function() {
		if((Lightbox.imageArray.length - 1) > Lightbox.activeImage){
			preloadNextImage = new Image();
			preloadNextImage.src = Lightbox.imageArray[parseInt(Lightbox.activeImage) + 1][0];
		}
		if(Lightbox.activeImage > 0){
			preloadPrevImage = new Image();
			preloadPrevImage.src = Lightbox.imageArray[parseInt(Lightbox.activeImage) - 1][0];
		}
	},

	//
	//	end()
	//
	end: function() {
		// Try to prevent multiple fadeouts on double-click.
		// $('#overlay, #lightbox, #loadingLink').unbind('click');
		
		Lightbox.disableKeyboardNav();
		$('#lightbox').hide();
		$("#overlay").fadeOut(Lightbox.overlaySpeed);
		$("select, object, embed").show();
	},
	
// -----------------------------------------------------------------------------------

	//
	// getPageScroll()
	// Returns array with x,y page scroll values.
	// Core code from - quirksmode.com
	//
	getPageScroll : function(){
		
		var xScroll, yScroll;

		if (self.pageYOffset) {
			yScroll = self.pageYOffset;
			xScroll = self.pageXOffset;
		} else if (document.documentElement && document.documentElement.scrollTop){	 // Explorer 6 Strict
			yScroll = document.documentElement.scrollTop;
			xScroll = document.documentElement.scrollLeft;
		} else if (document.body) {// all other Explorers
			yScroll = document.body.scrollTop;
			xScroll = document.body.scrollLeft;	
		}

		arrayPageScroll = new Array(xScroll,yScroll);
		return arrayPageScroll;
	},
	//
	// getPageSize()
	// Returns array with page width, height and window width, height
	// Core code from - quirksmode.com
	// Edit for Firefox by pHaez
	//
	getPageSize : function(){
		var xScroll, yScroll;

		if (window.innerHeight && window.scrollMaxY) {	
			xScroll = window.innerWidth + window.scrollMaxX;
			yScroll = window.innerHeight + window.scrollMaxY;
		} else if (document.body.scrollHeight > document.body.offsetHeight){ // all but Explorer Mac
			xScroll = document.body.scrollWidth;
			yScroll = document.body.scrollHeight;
		} else { // Explorer Mac...would also work in Explorer 6 Strict, Mozilla and Safari
			xScroll = document.body.offsetWidth;
			yScroll = document.body.offsetHeight;
		}

		var windowWidth, windowHeight;

		if (self.innerHeight) {	// all except Explorer
			if(document.documentElement.clientWidth){
				windowWidth = document.documentElement.clientWidth; 
			} else {
				windowWidth = self.innerWidth;
			}
			windowHeight = self.innerHeight;
		} else if (document.documentElement && document.documentElement.clientHeight) { // Explorer 6 Strict Mode
			windowWidth = document.documentElement.clientWidth;
			windowHeight = document.documentElement.clientHeight;
		} else if (document.body) { // other Explorers
			windowWidth = document.body.clientWidth;
			windowHeight = document.body.clientHeight;
		}	

		// for small pages with total height less then height of the viewport
		if(yScroll < windowHeight){
			pageHeight = windowHeight;
		} else { 
			pageHeight = yScroll;
		}


		// for small pages with total width less then width of the viewport
		if(xScroll < windowWidth){	
			pageWidth = xScroll;		
		} else {
			pageWidth = windowWidth;
		}

		arrayPageSize = new Array(pageWidth,pageHeight,windowWidth,windowHeight);
		return arrayPageSize;
	},
	//
	// pause(numberMillis)
	// Pauses code execution for specified time. Uses busy code, not good.
	// Help from Ran Bar-On [ran2103@gmail.com]
	//
	pause : function(ms){
		var date = new Date();
		curDate = null;
		do{var curDate = new Date();}
		while( curDate - date < ms);
	}
};

$(document).ready(function(){
	Lightbox.initialize();
});
