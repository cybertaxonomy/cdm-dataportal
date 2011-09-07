// $Id$
/**
* Copyright (C) 2011 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import org.openqa.selenium.WebElement;

/**
 * @author andreas
 * @date Aug 29, 2011
 *
 */
public class GalleryImage extends ImgElement {


	private WebElement captionElement;

	/**
	 * @param img
	 */
	public GalleryImage(WebElement img, WebElement caption) {
		super(img);
		this.captionElement = caption;
	}

	public String getCaptionText() {
		if(captionElement == null){
			return null;
		}
		return captionElement.getText();
	}

}
