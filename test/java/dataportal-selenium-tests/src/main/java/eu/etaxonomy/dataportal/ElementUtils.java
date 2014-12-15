// $Id$
/**
* Copyright (C) 2011 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal;

import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.ui.WebDriverWait;

import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.GalleryImage;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.pages.PortalPage;
import eu.etaxonomy.dataportal.selenium.AllTrue;
import eu.etaxonomy.dataportal.selenium.ChildElementVisible;
import eu.etaxonomy.dataportal.selenium.JUnitWebDriverWait;

/**
 * @author andreas
 * @date Sep 16, 2011
 *
 */
public class ElementUtils {


    public static final Logger logger = Logger.getLogger(ElementUtils.class);

    /**
     * @param fnListElements
     * @return
     */
    public static List<BaseElement> baseElementsFromFootNoteListElements(List<WebElement> fnListElements) {
        List<BaseElement> footNotes = new ArrayList<BaseElement>();
        for(WebElement fn : fnListElements) {
            footNotes.add(new BaseElement(fn));
        }
        return footNotes;
    }

    /**
     * @param fnkListElements
     * @return
     */
    public static List<LinkElement> linkElementsFromFootNoteKeyListElements(List<WebElement> fnkListElements) {
        List<LinkElement> footNoteKeys = new ArrayList<LinkElement>();
        for(WebElement fnk : fnkListElements) {
            footNoteKeys.add(new LinkElement(fnk));
        }
        return footNoteKeys;
    }

    /**
     * @param wait
     * @return a two dimensional array representing the media items in the gallery, or null if no gallery exists.
     */
    public static List<List<GalleryImage>> getGalleryImages(WebElement webElement, WebDriverWait wait) {


        WebElement gallery = webElement.findElement(By.className("media_gallery"));

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
