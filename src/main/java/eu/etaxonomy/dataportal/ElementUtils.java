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

import org.apache.commons.lang3.StringUtils;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.ui.WebDriverWait;

import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.GalleryImage;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.selenium.AllTrue;
import eu.etaxonomy.dataportal.selenium.ChildElementVisible;

/**
 * @author andreas
 * @since Sep 16, 2011
 */
public class ElementUtils {

    private static final Logger logger = LogManager.getLogger();

    public static List<BaseElement> baseElementsFromFootNoteListElements(List<WebElement> fnListElements) {
        List<BaseElement> footNotes = new ArrayList<>();
        for(WebElement fn : fnListElements) {
            footNotes.add(new BaseElement(fn));
        }
        return footNotes;
    }


    public static List<LinkElement> linkElementsFromFootNoteKeyListElements(List<WebElement> fnkListElements) {
        List<LinkElement> footNoteKeys = new ArrayList<>();
        for(WebElement fnk : fnkListElements) {
            footNoteKeys.add(new LinkElement(fnk));
        }
        return footNoteKeys;
    }

    /**
     * @param webElement the element containing the media gallery. The gallery must have the class attribute <code>media_gallery</code>.
     * @return a two dimensional array representing the media items in the gallery, or null if no gallery exists.
     */
    public static List<List<GalleryImage>> getGalleryImages(WebElement webElement, WebDriverWait wait) {


        WebElement gallery = webElement.findElement(By.className("media_gallery"));

        if( gallery == null){
            return null;
        }

        ArrayList<List<GalleryImage>> galleryImageRows = new ArrayList<>();

        List<WebElement> mediaRows = gallery.findElements(By.cssSelector("tr.media-row"));
        List<WebElement> captionRows = gallery.findElements(By.cssSelector("tr.caption-row"));
        logger.debug("GalleryImages - media rows: " + mediaRows.size() + " caption rows: " + captionRows.size());
        // loop table rows
        for(int rowId = 0; rowId < mediaRows.size(); rowId++ ){
            logger.debug("GalleryImages - gallery row " + rowId );
            List<WebElement> imageCells = mediaRows.get(rowId).findElements(By.tagName("td"));
            List<WebElement> captionCells = null;
            if(rowId < captionRows.size()) {
                captionCells = captionRows.get(rowId).findElements(By.tagName("td"));
                logger.debug("GalleryImages - image cells: " + imageCells.size() + " caption cells "+ captionCells.size());
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
                        // image must be visible
                        new ChildElementVisible(imageCell, By.tagName("img")),
                        // metadata should be loaded but may be empty, therefore we are tesing
                        // for the existence of the element attribute data-cdm-ahah-url-loaded
                        // in the <div class="ahah-content" ...
                        new ChildElementVisible(captionCell, By.xpath("div[@data-cdm-ahah-url-loaded]"))
                    ));

                } else {
                    wait.until(new ChildElementVisible(imageCell, By.tagName("img")));
                }
                galleryImageRows.get(rowId).add(new GalleryImage(imageCell, captionCell));
            }

        }

        return galleryImageRows;
    }

    /**
     * Expected DOM:
     *
     * <ul class="footnotes">
     *   <li class="footnotes footnotes-taxon_relationships "><span class="footnote footnote-1">...</span></li>
     *   <li class="footnotes footnotes-taxon_relationships-annotations ">...</li>
     * </ul>
     */
    public static List<BaseElement> findFootNotes(WebElement element){
        List<WebElement> fnListElements = element.findElements(
                By.xpath("./ul[contains(@class,'footnotes')]/li[contains(@class, 'footnotes')]/span[contains(@class, 'footnote')]")
        );
        return ElementUtils.baseElementsFromFootNoteListElements(fnListElements);
    }

    /**
     * Intended for logging purposes.
     * <p>
     * Creates incomplete markup of the WebElement with the attributes id and class like:
     * {@code <tagName id="the-id" class="">}
     *
     * @param we the WebElement
     * @return the markup
     */
    public static String webElementTagToMarkup(WebElement we) {
        String markup = "<" + we.getTagName();
        String idAttr = we.getAttribute("id");
        String classAttr = we.getAttribute("class");
        if(StringUtils.isNotEmpty(idAttr)) {
            markup += " id=\""+ classAttr + "\"";
        }
        if(StringUtils.isNotEmpty(classAttr)) {
            markup += " class=\""+ classAttr + "\"";
        }
        markup += ">";
        return markup;
    }
}