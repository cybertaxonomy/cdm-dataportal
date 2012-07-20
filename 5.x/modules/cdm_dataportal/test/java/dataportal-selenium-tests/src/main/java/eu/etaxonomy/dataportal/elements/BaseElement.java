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
import java.util.Arrays;
import java.util.List;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebElement;

/**
 * @author Andreas Kohlbecker
 * @date Jul 1, 2011
 *
 */
public class BaseElement {

    public static final Logger logger = Logger.getLogger(BaseElement.class);
    /**
     * Default tolerance for testing sizes and positions
     */
    protected static final double PIXEL_TOLERANCE = 0.5;

    private WebElement element;

    private List<String> classAttributes = null;

    private List<LinkElement> linksInElement = null;

    private String text = null;


    public WebElement getElement() {
        return element;
    }

    public String getText() {
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

            if(getElement().getTagName().equals("a") && getElement().getAttribute("href") != null){
                // BaseElement is link itself
                linksInElement.add(new LinkElement(getElement()));
            } else {
                // look for links in sub elements
                List<WebElement> links = getElement().findElements(By.xpath("./a[@href]"));
                for (WebElement link : links) {
                    linksInElement.add(new LinkElement(link));
                }
            }
        }
        return linksInElement;
    }

    public List<String> getLinkTargets(){

        List<String> targets = new ArrayList<String>();

        if(element instanceof LinkElement){
            targets.add(getElement().getAttribute("target"));
        } else {
            try {
                targets.add(getElement().findElement(By.xpath("./a[@target]")).getAttribute("target"));
            } catch (NoSuchElementException e) {
                logger.debug("No target window found");
                /* IGNORE */
            }
        }
        return targets;
    }

    /**
     * @param element
     */
    public BaseElement(WebElement element) {

        this.element = element;

        // read text
        text = element.getText();

        // read and tokenize the class attribute
        if (element.getAttribute("class") != null) {
            String[] classTokens = element.getAttribute("class").split("\\s");
            setClassAttributes(Arrays.asList(classTokens));
        }
    }

    /**
     * @return
     */
    public String toSting() {
        return this.getClass().getSimpleName() + "<" + this.getElement().getTagName() + ">" ;
    }

    /**
     *
     * @return
     */
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
        List<String> targets = getLinkTargets();
        return "<" + getElement().getTagName() + " class=\""+ getElement().getAttribute("class")+ "\"/>" + textSnipped + " ;Links:" + links + (targets.isEmpty()? "" : ", LinkTargets: '" + StringUtils.join(targets,	", ") + "'");
    }


}