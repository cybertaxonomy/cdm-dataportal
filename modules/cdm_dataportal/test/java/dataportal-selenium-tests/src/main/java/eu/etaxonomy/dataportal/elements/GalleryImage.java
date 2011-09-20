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

import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

/**
 * @author andreas
 * @date Aug 29, 2011
 *
 */
public class GalleryImage extends ImgElement {


	private WebElement captionCell;
	private WebElement mediaCell;

	/**
	 * @param img
	 */
	public GalleryImage(WebElement mediaCell, WebElement captionCell) {
		super(mediaCell.findElement(By.tagName("img")));
		this.captionCell = captionCell;
		this.mediaCell = mediaCell;
	}

	public String getCaptionText() {
		if(captionCell == null){
			return null;
		}
		return captionCell.findElement(By.tagName("dl")).getText();
	}

	public LinkElement getCaptionLink() {
		if(captionCell == null){
			return null;
		}
		return new LinkElement(captionCell.findElement(By.xpath("./div[contains(@class,'media-caption-link')]/a")));
	}

	public LinkElement getImageLink() {
		if(mediaCell == null){
			return null;
		}
		return new LinkElement(mediaCell.findElement(By.tagName("a")));
	}

}
