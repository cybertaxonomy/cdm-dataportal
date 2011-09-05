// $Id$
/**
* Copyright (C) 2011 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.pages;

import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.CacheLookup;
import org.openqa.selenium.support.FindBy;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.TaxonListElement;
import eu.etaxonomy.dataportal.selenium.AllTrue;
import eu.etaxonomy.dataportal.selenium.PageTitleValidated;
import eu.etaxonomy.dataportal.selenium.VisibilityOfElementLocated;

/**
 * @author andreas
 * @date Aug 12, 2011
 *
 */
public class TaxonSearchResultPage extends GenericPortalPage {

	public static final Logger logger = Logger.getLogger(TaxonSearchResultPage.class);

	private static String drupalPagePathBase = "cdm_dataportal/search/taxon";

	@FindBy(id="search_results")
	@CacheLookup
	private WebElement searchResults;

	//class=page_options
	@FindBy(className="page_options")
	@CacheLookup
	private WebElement pageOptions;

	//class=pager
	@FindBy(className="pager")
	@CacheLookup
	private WebElement pager;


	/* (non-Javadoc)
	 * @see eu.etaxonomy.dataportal.pages.PortalPage#getDrupalPageBase()
	 */
	@Override
	protected String getDrupalPageBase() {
		return drupalPagePathBase;
	}

	/**
	 * @param driver
	 * @param context
	 * @throws Exception
	 */
	public TaxonSearchResultPage(WebDriver driver, DataPortalContext context) throws Exception {
		super(driver, context);
	}


	/**
	 * Find and return the result n-th item of the result list.
	 * The item can be specified by the index paramter.
	 * @param index 1-based index to identify the resultset item.
	 * This index will be used in a xpath query.
	 * @return
	 */
	public TaxonListElement getResultItem(int index) {

		TaxonListElement entry = new TaxonListElement(searchResults.findElement(By.xpath("ul/li[" + index + "]")));

		return entry;
	}

	/**
	 * Find and returns all items of the result list.
	 * @return
	 */
	public List<TaxonListElement> getResultItems() {

		List<WebElement> entryList = searchResults.findElements(By.xpath("/ul/li"));
		List<TaxonListElement> taxonListElements = new ArrayList<TaxonListElement>();
		for(WebElement entry : entryList){
			taxonListElements.add(new TaxonListElement(entry));
		}
		return taxonListElements;
	}

	/**
	 * @throws Exception
	 *
	 */
	public <T extends PortalPage> T  clickTaxonName(TaxonListElement taxonListElement) throws Exception {

		WebElement taxonlink = taxonListElement.getElement().findElement(By.tagName("a"));
		logger.info("clicking on link to " + taxonlink.getAttribute("href"));
		taxonlink.click();
		wait.until(new AllTrue(new PageTitleValidated(context.prepareTitle("Illicium")), new VisibilityOfElementLocated(By.id("container"))));
		return (T) new TaxonProfilePage(driver, context);

	}



}
