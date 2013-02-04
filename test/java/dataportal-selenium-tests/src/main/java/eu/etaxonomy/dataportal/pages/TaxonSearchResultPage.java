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
import java.util.concurrent.TimeUnit;

import org.apache.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.CacheLookup;
import org.openqa.selenium.support.FindBy;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.GalleryImage;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.elements.TaxonListElement;
import eu.etaxonomy.dataportal.selenium.AllTrue;
import eu.etaxonomy.dataportal.selenium.ChildElementVisible;
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
    @SuppressWarnings("unchecked")
    public <T extends PortalPage> T  clickTaxonName(TaxonListElement taxonListElement, Class<T> pageClass) throws Exception {

        LinkElement taxonlink = new LinkElement(taxonListElement.getElement().findElement(By.tagName("a")));
        logger.debug("taxonlink to click: " + taxonlink.toString() + " [" + taxonlink.getElement().toString() + "]");
        return (T) clickLink(taxonlink, new VisibilityOfElementLocated(By.id("featureTOC")), pageClass, 2l, TimeUnit.MINUTES);

    }

    /**
     * @return a two dimensional array representing the media items in the gallery, or null if no gallery exists.
     */
    public List<List<GalleryImage>> getGalleryImagesOf(TaxonListElement taxonListElement) {


        WebElement gallery = taxonListElement.getElement().findElement(By.className("media_gallery"));

        if( gallery == null){
            return null;
        }

        ArrayList<List<GalleryImage>> galleryImageRows = new ArrayList<List<GalleryImage>>();

        List<WebElement> tableRows = gallery.findElements(By.tagName("tr"));
        logger.debug("GalleryImages - total rows " + tableRows.size());
        // loop table rows
        for(int rowId = 0; rowId * 2 < tableRows.size() && tableRows.size() > 0; rowId++ ){
            logger.debug("GalleryImages - gallery row " + rowId );
            List<WebElement> imageCells = tableRows.get(rowId * 2).findElements(By.tagName("td"));
            logger.debug("GalleryImages - number of image cells: " + imageCells.size());
            List<WebElement> captionCells = null;
            if(tableRows.size() > rowId * 2 + 1){
                captionCells = tableRows.get(rowId * 2 + 1).findElements(By.tagName("td"));
                logger.debug("GalleryImages - number of caption cells: " + captionCells.size());
            }

            galleryImageRows.add(new ArrayList<GalleryImage>());

            // loop table cells in row
            for(int cellId = 0; cellId < imageCells.size(); cellId++) {
                logger.debug("cellId:" + cellId);
                WebElement imageCell = imageCells.get(cellId);
                WebElement captionCell = null;
                if(captionCells != null && captionCells.size() > cellId){
                    captionCell = captionCells.get(cellId);
                    wait.until(new AllTrue(
                            new ChildElementVisible(imageCell, By.tagName("img")),
                            new ChildElementVisible(captionCell, By.tagName("dl"))
                    ));

                } else {
                    wait.until(new ChildElementVisible(imageCell, By.tagName("img")));
                }
                galleryImageRows.get(rowId).add(new GalleryImage(imageCell, captionCell));
            }

        }

        return galleryImageRows;
    }



}
