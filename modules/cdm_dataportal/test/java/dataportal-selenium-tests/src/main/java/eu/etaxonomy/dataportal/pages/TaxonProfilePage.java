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
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import org.apache.log4j.Logger;
import org.junit.Assert;
import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.RenderedWebElement;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.CacheLookup;
import org.openqa.selenium.support.FindBy;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.DrupalBlock;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.elements.ImgElement;
import eu.etaxonomy.dataportal.elements.LinkElement;

/**
 * @author andreas
 * @date Jul 1, 2011
 *
 */
public class TaxonProfilePage extends PortalPage {

	public static final Logger logger = Logger.getLogger(TaxonProfilePage.class);

	private UUID taxonUuid;

	protected static String drupalPagePathBase = "cdm_dataportal/taxon";

	/* (non-Javadoc)
	 * @see eu.etaxonomy.dataportal.pages.PortalPage#getDrupalPageBase()
	 */
	@Override
	protected String getDrupalPageBase() {
		return drupalPagePathBase;
	}

	@FindBy(id = "taxonProfileImage")
	@CacheLookup
	private RenderedWebElement taxonProfileImage;

	@FindBy(id = "featureTOC")
	@CacheLookup
	private RenderedWebElement tableOfContent;



	public TaxonProfilePage(WebDriver driver, DataPortalContext context, UUID taxonUuid) throws MalformedURLException {

		super(driver, context, taxonUuid.toString());

		this.taxonUuid = taxonUuid;
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

	public String getTableOfContentHeader() {
		if(tableOfContent != null) {
			WebElement header = tableOfContent.findElement(By.tagName("h2"));
			return header.getText();
		}
		return null;
	}

	public List<LinkElement> getTableOfContentLinks() {
		List<LinkElement> linkList = null;
		if(tableOfContent != null) {
			linkList = new ArrayList<LinkElement>();
			List<WebElement> listItems = tableOfContent.findElements(By.tagName("a"));
			for (WebElement li : listItems) {
				linkList.add( new LinkElement((RenderedWebElement) li) );
			}
		}
		return linkList;
	}

	public FeatureBlock getFeatureBlockAt(int position, String featureName, String enclosingTag, String elementTag){

		List<WebElement> featureBlocks = portalContent.findElements(By.className("block-cdm_dataportal-feature"));
		Assert.assertTrue("Too few feature block elements", featureBlocks.size() >= position);
		for(WebElement b : featureBlocks){
			if (b.getAttribute("id").equals("block-cdm_dataportal-feature-" + featureName)){
				return new FeatureBlock((RenderedWebElement) b, enclosingTag, elementTag);
			}
		}
		return null;
	}

}
