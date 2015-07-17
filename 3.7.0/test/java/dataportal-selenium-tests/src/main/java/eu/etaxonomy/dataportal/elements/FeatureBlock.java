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

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import static junit.framework.Assert.*;
import static org.junit.Assert.assertEquals;

import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.WebElement;

import com.gargoylesoftware.htmlunit.ElementNotFoundException;

/**
 * @author andreas
 * @date Jul 4, 2011
 *
 */
public class FeatureBlock extends DrupalBlock {

    private List<LinkElement> footNoteKeys = new ArrayList<LinkElement>();

    private List<BaseElement> footNotes = new ArrayList<BaseElement>();

    private List<BaseElement> originalSources = new ArrayList<BaseElement>();

    private List<DescriptionElementRepresentation> descriptionElements = new ArrayList<DescriptionElementRepresentation>();

    private String featureType = null;


    public List<LinkElement> getFootNoteKeys() {
        return footNoteKeys;
    }

    public List<BaseElement> getFootNotes() {
        return footNotes;
    }

    public List<BaseElement> getOriginalSourcesSections() {
        return originalSources;
    }


    public List<DescriptionElementRepresentation> getDescriptionElements() {
        return descriptionElements;
    }

    public String getFeatureType() {
        return featureType;
    }


    /**
     * @param element
     */
    public FeatureBlock(WebElement element, String enclosingTag, String ... elementTags) {
        super(element);

        List<WebElement> fnkList = element.findElements(By.className("footnote-key"));
        for(WebElement fnk : fnkList) {
            footNoteKeys.add(new LinkElement(fnk.findElement(By.tagName("a"))));
        }

        List<WebElement> fnList = element.findElements(By.className("footnote"));
        for(WebElement fn : fnList) {
            footNotes.add(new BaseElement(fn));
        }

        List<WebElement> sourcesList = element.findElements(By.className("sources"));
        for(WebElement source : sourcesList) {
            originalSources.add(new BaseElement(source));
        }

        WebElement descriptionElementsRepresentation =  element.findElement(By.className("feature-block-elements"));
        featureType = descriptionElementsRepresentation.getAttribute("id");

        //TODO throw exception instead of making an assertion! selenium should have appropriate exceptions
        assertEquals("Unexpected tag enclosing description element representations", enclosingTag, descriptionElementsRepresentation.getTagName());

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
                List<WebElement> elementsByIndex = new ArrayList<WebElement>();
                for (String elementTag : elementTags) {
                    elementsByIndex.add(elementsByTag.get(elementTag).get(descriptionElementIndex));
                }
                descriptionElements.add(new MultipartDescriptionElementRepresentation(elementsByIndex.toArray(new WebElement[elementsByIndex.size()])));

            }
        } else {
            // handle single elements
            String elementTag = elementTags[0];
            for(WebElement el : descriptionElementsRepresentation.findElements(By.tagName( elementTag ))) {
                descriptionElements.add(new DescriptionElementRepresentation(el));
            }
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

        DescriptionElementRepresentation firstDescriptionElement = getDescriptionElements().get(descriptionElementId);

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
