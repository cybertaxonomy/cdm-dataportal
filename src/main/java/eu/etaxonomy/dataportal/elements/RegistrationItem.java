/**
* Copyright (C) 2019 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import java.util.ArrayList;
import java.util.List;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

/**
 * @author a.kohlbecker
 * @since Feb 4, 2019
 *
 */
abstract public class RegistrationItem extends BaseElement {

    private static final Logger logger = LogManager.getLogger();

    enum Style {
        /**
         * style produced by modules/cdm_dataportal/includes/name.inc#compose_registration_dto_full()
         */
        FULL,
        /**
         * style produced by modules/cdm_dataportal/includes/name.inc#compose_registration_dto_compact()
         */
        COMPACT
    }

    protected Style style = null;
    protected List<BaseElement> specimenTypeDesignations;
    protected List<BaseElement> nameTypeDesignations;
    protected WebElement citation;
    protected WebElement metadata;
    protected WebElement identifier;
    protected WebElement nameElement;
    protected List<BaseElement> nameRelationsipsElements;
    protected List<BaseElement> registrationFootnotes;
    protected WebElement typifiedNameElement;
    protected WebElement summaryElement;


    protected RegistrationItem(WebElement containerElement) {
        super(containerElement);

        try {
            summaryElement = containerElement.findElement(By.cssSelector(".registration-summary"));
            style = Style.COMPACT;
            identifier = containerElement.findElement(By.cssSelector(".identifier"));
        } catch (Exception e) {
            logger.debug("web element .registration-summary not found, now trying full style elements");
        }

        if(style == null){
            try {
                nameElement = containerElement.findElement(By.cssSelector(".published-name"));
            } catch (Exception e) {
                try {
                    typifiedNameElement = containerElement.findElement(By.cssSelector(".typified-name"));
                } catch (Exception e2) {
                    // typifiedNameElement must exist when nameElement is not present, so we throw the  Exception in this case:
                    throw e2;
                }
            }
            try{
                List<WebElement> std = containerElement.findElements(By.cssSelector(".name_relationships .item"));
                nameRelationsipsElements = new ArrayList<BaseElement>(std.size());
                for(WebElement we : std){
                    nameRelationsipsElements.add(new BaseElement(we));
                }
            } catch (Exception e) { /* IGNORE */}
            try{
                List<WebElement> std = containerElement.findElements(By.cssSelector(".footnotes-nomenclatural_act .footnote"));
                registrationFootnotes = new ArrayList<BaseElement>(std.size());
                for(WebElement we : std){
                    registrationFootnotes.add(new BaseElement(we));
                }
            } catch (Exception e) { /* IGNORE */}
            try{
                List<WebElement> std = containerElement.findElements(By.cssSelector(".cdm\\:SpecimenTypeDesignation"));
                specimenTypeDesignations = new ArrayList<BaseElement>(std.size());
                for(WebElement we : std){
                    specimenTypeDesignations.add(new BaseElement(we));
                }
            } catch (Exception e) { /* IGNORE */}
            try {
                List<WebElement> ntd = containerElement.findElements(By.cssSelector(".name_type_designation"));
                nameTypeDesignations = new ArrayList<BaseElement>(ntd.size());
                for(WebElement we : ntd){
                    nameTypeDesignations.add(new BaseElement(we));
                }
            } catch (Exception e) { /* IGNORE */}
            try {
                citation = containerElement.findElement(By.cssSelector(".citation"));
                style = Style.FULL;
            } catch (Exception e) { /* IGNORE */}


        }
        // now the general elements which must exist
        metadata = containerElement.findElement(By.cssSelector(".registration-date-and-institute"));
    }


    public Style getStyle() {
        return style;
    }

    public WebElement getMetadata() {
        return metadata;
    }

    public WebElement getIdentifier() {
        return identifier;
    }



}
