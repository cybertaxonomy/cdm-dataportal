/**
* Copyright (C) 2019 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import java.util.List;

import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

/**
 * @author a.kohlbecker
 * @since Feb 4, 2019
 *
 */
abstract public class RegistrationItem extends BaseElement {

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
    protected List<WebElement> specimenTypeDesignations;
    protected List<WebElement> nameTypeDesignations;
    protected WebElement citation;
    protected WebElement metadata;
    protected WebElement identifier;
    protected WebElement nameElement;
    protected WebElement summaryElement;

    /**"
     * @param element
     */
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
                nameElement = containerElement.findElement(By.cssSelector(".name"));
            } catch (Exception e) { /* IGNORE */}
            try{
                specimenTypeDesignations = containerElement.findElements(By.cssSelector(".specimen_type_designation"));
            } catch (Exception e) { /* IGNORE */}
            try {
                nameTypeDesignations = containerElement.findElements(By.cssSelector(".name_type_designation"));
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
