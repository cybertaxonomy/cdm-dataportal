// $Id$

/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.pages;

import java.net.MalformedURLException;

import org.apache.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.RenderedWebElement;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.CacheLookup;
import org.openqa.selenium.support.FindBy;

import eu.etaxonomy.dataportal.elements.ImgElement;

/**
 * @author andreas
 * @date Jul 1, 2011
 *
 */
public class TaxonProfilePage extends PortalPage {

	public static final Logger logger = Logger.getLogger(TaxonProfilePage.class);

	@FindBy(id = "taxonProfileImage")
	@CacheLookup
	private RenderedWebElement taxonProfileImage;

	public TaxonProfilePage(WebDriver driver) throws MalformedURLException {
		super(driver);
	}

	/**
	 * Returns the profile image of the taxon profile page. This image is
	 * located at the top of the page. The Profile Image can be disabled in the
	 * DataPortal settings.
	 *
	 * @return The Url of the profile image or null if the image is not visible.
	 */
	public ImgElement getProfileImage() {
		ImgElement imgElement = null;
		try {
			if(taxonProfileImage.isDisplayed()){
				RenderedWebElement img = (RenderedWebElement) taxonProfileImage.findElement(By.tagName("img"));
				if (img != null) {
					imgElement = new ImgElement(img);
				}
			}
		} catch (NoSuchElementException e) {
			// IGNORE //
		}
		return imgElement;
	}
}
