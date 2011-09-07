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

import java.util.List;

import javax.swing.Box.Filler;

import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.pages.GenericPortalPage;
import eu.etaxonomy.dataportal.pages.PortalPage;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;
import eu.etaxonomy.dataportal.pages.TaxonSearchResultPage;
import eu.etaxonomy.dataportal.selenium.AllTrue;
import eu.etaxonomy.dataportal.selenium.JUnitWebDriverWait;
import eu.etaxonomy.dataportal.selenium.PageTitleValidated;
import eu.etaxonomy.dataportal.selenium.VisibilityOfElementLocated;

/**
 * @author andreas
 * @date Aug 29, 2011
 *
 */
public class TaxonListElement extends BaseElement {

	private TaxonType type;

	private String fullTaxonName;


	/**
	 * @param element
	 */
	public TaxonListElement(WebElement element) {
		super(element);
		type = TaxonType.valueOf( element.getAttribute("class") );
		fullTaxonName = element.findElement(By.tagName("span")).getText();
	}


	public TaxonType getType() {
		return type;
	}


	public String getFullTaxonName() {
		return fullTaxonName;
	}

	/**
	 * @return a two dimensional array representing the media items in the gallery, or null if no gallery exists.
	 */
	public GalleryImage[][] getGalleryImages() {


		WebElement gallery = getElement().findElement(By.className("media_gallery"));

		if( gallery == null){
			return null;
		}

		GalleryImage[][] galleryImages = new GalleryImage[][]{};

		List<WebElement> tableRows = gallery.findElements(By.tagName("tr"));

		// loop table rows
		for(int rowId = 0; rowId < tableRows.size(); rowId++ ){
			List<WebElement> imageCells = tableRows.get(rowId * 2).findElements(By.tagName("td"));
			List<WebElement> captionCells = null;
			if(tableRows.size() > rowId * 2 + 1){
				captionCells = tableRows.get(rowId * 2 + 1).findElements(By.tagName("td"));
			}

			galleryImages[rowId] = new GalleryImage[]{};

			// loop table cells in row
			for(int cellId = 0; cellId < imageCells.size(); cellId++) {
				WebElement image = imageCells.get(cellId).findElement(By.tagName("img"));
				WebElement caption = null;
				if(captionCells != null && captionCells.size() > cellId){
					caption = captionCells.get(cellId).findElement(By.tagName("dl"));
				}
				galleryImages[rowId][cellId] = new GalleryImage(image, caption);
			}

		}

		return null;
	}




}
