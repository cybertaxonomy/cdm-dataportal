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
import java.util.Arrays;
import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebDriverException;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.ui.ExpectedCondition;
import org.openqa.selenium.support.ui.WebDriverWait;

import eu.etaxonomy.dataportal.ElementUtils;

/**
 * @author Andreas Kohlbecker
 * @since Jul 1, 2011
 */
public class BaseElement {

    private static final Logger logger = LogManager.getLogger();

    /**
     * Default tolerance for testing sizes and positions
     */
    protected static final double PIXEL_TOLERANCE = 0.5;

    private final WebElement element;

    private List<String> classAttributes = null;

    private List<LinkElement> linksInElement = null;

    List<String> linkTargets = null;

    private String text = null;


    public WebElement getElement() {
        return element;
    }

    /**
     * Null save factory method
     *
     * @param we
     *  May be <code>null</code>
     * @return
     *  The new BaseElement or <code>null</code>.
     */
    public static BaseElement from(WebElement we) {
        if(we != null) {
            return new BaseElement(we);
        }
        return null;
    }

    public String getText() {
        if(text == null) {
            text = element.getText();
        }
        return text;
    }

    public List<String> getClassAttributes() {
        return classAttributes;
    }

    void setClassAttributes(List<String> classAttributes) {
        this.classAttributes = classAttributes;
    }

    public double getComputedFontSize(){
        return pxSizeToDouble(getElement().getCssValue("font-size") );
    }

    public static double pxSizeToDouble(String pxValue){
        return Double.valueOf( pxValue.replaceAll("[a-zA-Z]", "") );
    }

    /**
     * Finds and returns links of the form &lt;a href="LINK" /&gt; in this element or in any child element.
     *
     * @return an array of {@link LinkElement} objects, the array is empty if no link has been found
     */
    public List<LinkElement> getLinksInElement() {
        if(linksInElement == null){
            linksInElement = new ArrayList<LinkElement>();

            if(getElement().getTagName().equals("a") && getElement().getAttribute("href") != null && getElement().getAttribute("href").length() > 0){
                // BaseElement is link itself
                linksInElement.add(new LinkElement(getElement()));
            } else {
                // look for links in sub elements
                List<WebElement> links = getElement().findElements(By.xpath(".//a[@href]"));
                for (WebElement link : links) {
                    linksInElement.add(new LinkElement(link));
                }
            }
        }
        return linksInElement;
    }

    public List<LinkElement> getFootNoteKeys(){
        return ElementUtils.linkElementsFromFootNoteKeyListElements(
                getElement().findElements(By.xpath(".//*[contains(@class, 'footnote-key')]/a"))
                );
    }

    public List<BaseElement> getFootNotes(){
        return ElementUtils.baseElementsFromFootNoteListElements(
                // NOTE: the training space character in 'footnote ' is important. Without it would also match the footnote-anchor!
                getElement().findElements(By.xpath("//*[contains(@class, 'footnotes')]/span[contains(@class, 'footnote ')]"))
                );
    }


    public BaseElement getFootNoteForKey(LinkElement footNoteKey){
        String key = footNoteKey.getText();
        List<BaseElement> matchingFootnotes = ElementUtils.baseElementsFromFootNoteListElements(
                // NOTE: the training space character in 'footnote ' is important. Without it would also match the footnote-anchor!
                getElement().findElements(By.cssSelector("span.footnotes span.footnote-" + key)
                        )
                );
        assert matchingFootnotes.size() == 1;
        return matchingFootnotes.get(0);

    }


    /**
     *
     * @param driver the currently used  WebDriver instance
     * @return list of linktargets
     */
    public List<String> getLinkTargets(WebDriver driver){

        if(linkTargets == null){
            linkTargets = new ArrayList<String>();
            if(getElement().getTagName().equals("a") && getElement().getAttribute("target") != null  && getElement().getAttribute("target").length() > 0){
                linkTargets.add(getElement().getAttribute("target"));
            } else {
                try {
                    WebDriverWait wait = new WebDriverWait(driver, 5);

                    List<WebElement> anchorTags = wait.until(new ExpectedCondition<List<WebElement>>(){
                        @Override
                        public List<WebElement> apply(WebDriver d) {
                            return d.findElements(By.tagName("a"));
                        }
                      }
                    );

                    WebElement linkWithTarget = null;
                    if(anchorTags.size() > 0){
                        if(anchorTags.get(0).getAttribute("target") != null) {
                            linkWithTarget = anchorTags.get(0);
                        }
                    }

                    if(linkWithTarget != null){ // assuming that there is only one
                        linkTargets.add(linkWithTarget.getAttribute("target"));
                    }
                } catch (NoSuchElementException e) {
                    logger.debug("No target window found");
                    /* IGNORE */
                } catch (WebDriverException e) {
                    logger.debug("timed out", e);
                    /* IGNORE */
                }
            }
        }
        return linkTargets;
    }


    public BaseElement(WebElement element) {

        logger.trace("BaseElement() - constructor");
        this.element = element;

        logger.debug("wrapping " + ElementUtils.webElementTagToMarkup(getElement()));
        // read and tokenize the class attribute
        if (element.getAttribute("class") != null) {
            String[] classTokens = element.getAttribute("class").split("\\s");
            setClassAttributes(Arrays.asList(classTokens));
            logger.trace("BaseElement() - class attribute loaded");
        }
    }


    @Override
    public String toString() {
        return this.getClass().getSimpleName() + ":" + ElementUtils.webElementTagToMarkup(getElement()) ;
    }

    public String toStringWithLinks() {

        StringBuilder links = new StringBuilder();
        for(LinkElement linkElement : getLinksInElement()){
            if(links.length() > 0){
                links.append(", ");
            }
            links.append(linkElement.getUrl());
        }
        String classAttribute = getElement().getAttribute("class") ;
        if(classAttribute == null){
            classAttribute = "";
        }
        String textSnipped = getText();
        if(textSnipped.length() > 50){
            textSnipped = textSnipped.substring(0, 50) + "...";
        }
        String targets = "LinkTargets UN-INITIALIZED";
        if(linkTargets != null) {
            targets = linkTargets.isEmpty()? "" : ", LinkTargets: '" + StringUtils.join(linkTargets.toArray(),	", ") + "'";
        }
        return "<" + getElement().getTagName() + " class=\""+ getElement().getAttribute("class")+ "\"/>" + textSnipped + " ;Links:" + links + targets;
    }

}
