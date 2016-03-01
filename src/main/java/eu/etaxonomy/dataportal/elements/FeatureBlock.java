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

import static org.junit.Assert.assertEquals;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.openqa.selenium.By;
import org.openqa.selenium.JavascriptExecutor;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;

/**
 * @author andreas
 * @date Jul 4, 2011
 *
 */
public class FeatureBlock extends DrupalBlock {

    /**
     * JQuery Selector for footnotekeys
     */
    private static final String SELECTOR_FOOTNOTE_KEY = "span.footnote-key a";

    /**
     * JQuery Selector for footnotes
     */
    private static final String SELECTOR_FOOTNOTE = "span.footnote";

    private final WebDriver driver;

    private List<BaseElement> originalSources = null;

    private String featureType = null;

    private final String[] elementTags;

    private final List<WebElement> descriptionItemElements;


    /**
     *
     * @param index 0 based
     * @return
     */
    public LinkElement getFootNoteKey(int index) {
        WebElement footNoteKeyElement = jsGetElement(SELECTOR_FOOTNOTE_KEY, index);
        if(footNoteKeyElement != null) {
            return new LinkElement(footNoteKeyElement);
        } else {
            return null;
        }
    }

    public boolean hasFootNoteKeys() {
        return countFootNoteKeys() > 0;
    }

    public long countFootNoteKeys() {
        return jsCountElements(SELECTOR_FOOTNOTE_KEY);
    }

    /**
     *
     * @param index 0 based
     * @return
     */
    public BaseElement getFootNote(int index) {
        WebElement footNoteElement = jsGetElement(SELECTOR_FOOTNOTE, index);
        if(footNoteElement != null) {
            return new BaseElement(footNoteElement);
        } else {
            return null;
        }
    }


    public long countFootNotes() {
        return jsCountElements(SELECTOR_FOOTNOTE);
    }

    public boolean hasFootNotes() {
        return countFootNotes() > 0;
    }

    public List<BaseElement> getOriginalSourcesSections() {
        if(originalSources == null) {
            initSources();
        }
        return originalSources;
    }


    public DescriptionElementRepresentation getDescriptionElement(int index) {
        WebElement descriptionElement = descriptionItemElements.get(index);
        if(descriptionElement != null) {
            if(elementTags.length > 1){
                // it is a multipart element e.g. <dt></dt><dd></dd>
                return new MultipartDescriptionElementRepresentation(
                        descriptionItemElements.toArray(new WebElement[descriptionItemElements.size()]));
            } else {
                return new DescriptionElementRepresentation(descriptionElement);
            }
        }
        return null;
    }

    public String getFeatureType() {
        return featureType;
    }


    /**
     * @param driver TODO
     * @param element
     */
    public FeatureBlock(WebDriver driver, WebElement element, String enclosingTag, String ... elementTags) {

        super(element);
//        logger.setLevel(Level.TRACE);
        logger.trace("FeatureBlock() - constructor after super()");

        this.driver = driver;
        this.elementTags = elementTags;

        WebElement descriptionElementsRepresentation =  element.findElement(By.className("feature-block-elements"));
        featureType = descriptionElementsRepresentation.getAttribute("id");

        //TODO throw exception instead of making an assertion! selenium should have appropriate exceptions
        assertEquals("Unexpected tag enclosing description element representations", enclosingTag, descriptionElementsRepresentation.getTagName());

        logger.trace("FeatureBlock() - loading all elements ...");
        descriptionItemElements = new ArrayList<WebElement>();
        if(elementTags.length > 1){

            // handle multipart elements e.g. <dt></dt><dd></dd>
            HashMap<String, List<WebElement>> elementsByTag = new HashMap<String, List<WebElement>>();
            Integer lastSize = null;
            for (String elementTag : elementTags) {
                List<WebElement> foundElements = descriptionElementsRepresentation.findElements(By.tagName(elementTag));
                if(lastSize != null && foundElements.size() != lastSize){
                    throw new NoSuchElementException("Mulitpart element lists differ in size");
                }
                lastSize = foundElements.size();
                elementsByTag.put(elementTag, foundElements);
            }

            for (int descriptionElementIndex = 0; descriptionElementIndex < lastSize; descriptionElementIndex++){
                for (String elementTag : elementTags) {
                    descriptionItemElements.add(elementsByTag.get(elementTag).get(descriptionElementIndex));
                }
//                descriptionElements.add(new MultipartDescriptionElementRepresentation(descriptionItemElements.toArray(new WebElement[descriptionItemElements.size()])));

            }
        } else {
            // handle single elements
            String elementTag = elementTags[0];
            for(WebElement el : descriptionElementsRepresentation.findElements(By.tagName( elementTag ))) {
                descriptionItemElements.add(el);
//                descriptionElements.add(new DescriptionElementRepresentation(el));
            }
        }
        logger.trace("FeatureBlock() - loading all elements DONE");

    }

    /**
     * @return
     */
    private Long jsCountElements(String jQuerySelector) {
        if(driver instanceof JavascriptExecutor) {
            String blockId = getElement().getAttribute("id");
            if(!blockId.startsWith("block-cdm-dataportal-feature-")) {
                throw new IllegalStateException("The block with id " + blockId + " is not a proper feature block");
            }
            // NOTE: jQuery(document).ready() must not be used here, otherwise
            // the executeScript() function will return null
            String js = "var elementCnt = jQuery('#" + blockId + "')."
                    + "    find('" + jQuerySelector + "').length;"
//                    + "  // console.log('count is ' + elementCnt);"
                    + "return elementCnt;";
            Object resultO  = ((JavascriptExecutor) driver).executeScript(js);
            logger.debug("FootNoteKeys count is " + resultO);
            if(resultO !=  null) {
                return Long.valueOf(resultO.toString());
            }
            return null;
        }
        throw new IllegalStateException("The driver must be a JavascriptExecutor");
    }

    /**
     * @param elementIndex TODO
     * @return
     */
    private WebElement jsGetElement(String jQuerySelector, int elementIndex) {
        if(driver instanceof JavascriptExecutor) {
            String blockId = getElement().getAttribute("id");
            if(!blockId.startsWith("block-cdm-dataportal-feature-")) {
                throw new IllegalStateException("The block with id " + blockId + " is not a proper feature block");
            }
            // NOTE: jQuery(document).ready() must not be used here, otherwise
            // the executeScript() function will return null
            String js = "var elements = jQuery('#" + blockId + "')."
                            + "   find('" + jQuerySelector + "');"
//                            + "console.log(elements.length);"
                            + "return elements[" + elementIndex +"];";
            Object resultO  = ((JavascriptExecutor) driver).executeScript(js);
            logger.debug("FootNoteKeys count is " + resultO);
            if(resultO instanceof WebElement) {
                return (WebElement)resultO;
            }
            if(resultO instanceof List) {
                throw new IllegalStateException("The selector '" + jQuerySelector + "' matches multiple elements, this is not allowed.");
            }
            return null;
        }
        throw new IllegalStateException("The driver must be a JavascriptExecutor");
    }

    /**
     * @param element
     */
    private void initSources() {
        originalSources = new ArrayList<BaseElement>();
        List<WebElement> sourcesList = getElement().findElements(By.className("sources"));
        for(WebElement source : sourcesList) {
            originalSources.add(new BaseElement(source));
        }
    }

    /**
     * @param indent
     * @param computedFontSize
     * @param expectedCssDisplay
     * @param expectedListStyleType only applies if cssDisplay equals list-item
     * @param expectedListStylePosition only applies if cssDisplay equals list-item
     * @param expectedListStyleImage only applies if cssDisplay equals list-item
     */
    public void testDescriptionElementLayout(int descriptionElementId, int indent, int computedFontSize
            , String expectedCssDisplay, String expectedListStyleType, String expectedListStylePosition, String expectedListStyleImage) {

        DescriptionElementRepresentation firstDescriptionElement = getDescriptionElement(descriptionElementId);

        if(firstDescriptionElement instanceof MultipartDescriptionElementRepresentation){
            int multipartElementIndex = 0;
            firstDescriptionElement = ((MultipartDescriptionElementRepresentation)firstDescriptionElement).multipartElements.get(multipartElementIndex);
        }
        int parentX = getElement().getLocation().getX();
        int elementX = firstDescriptionElement.getElement().getLocation().getX();
        double elementPadLeft = pxSizeToDouble(firstDescriptionElement.getElement().getCssValue("padding-left"));

        assertEquals(indent, elementX - parentX + elementPadLeft, PIXEL_TOLERANCE);
        assertEquals("css font-size:", computedFontSize, firstDescriptionElement.getComputedFontSize(), 0.5);
        assertEquals("css display:", expectedCssDisplay, firstDescriptionElement.getElement().getCssValue("display"));

        if(expectedCssDisplay.equals("list-item")){
            assertEquals("css list-style-position: ", expectedListStylePosition, firstDescriptionElement.getElement().getCssValue("list-style-position"));
            assertEquals("css list-style-image: ",  expectedListStyleImage, firstDescriptionElement.getElement().getCssValue("list-style-image"));
            assertEquals("css list-style-type: ", expectedListStyleType, firstDescriptionElement.getElement().getCssValue("list-style-type"));
        }
    }

    public List<GalleryImage> getGalleryMedia() {
        List<GalleryImage> galleryImages = null; //getGalleryImages(getElement());

        return galleryImages;

    }

}
