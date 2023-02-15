/**
* Copyright (C) 2019 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.pages;

import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.UUID;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.CacheLookup;
import org.openqa.selenium.support.FindBy;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.TypeDesignationElement;

/**
 * @author a.kohlbecker
 * @since Feb 4, 2019
 */
public class NamePage extends PortalPage {

    @SuppressWarnings("unused")
    private static final Logger logger = LogManager.getLogger();

    protected static String drupalPagePathBase = "cdm_dataportal/name";

    @FindBy(className = "typeDesignations")
    @CacheLookup
    private List<WebElement> typeDesignationContainers;

    private List<List<TypeDesignationElement>> typeDesignationElementsByContainer = null;

    public NamePage(WebDriver driver, DataPortalContext context) throws Exception {
        super(driver, context);
    }

    public NamePage(WebDriver driver, DataPortalContext context, UUID nameUuid) throws MalformedURLException {
        super(driver, context, nameUuid.toString());
    }

    @Override
    protected String getDrupalPageBase() {
        return drupalPagePathBase;
    }

    /**
     * @return the registrationItem
     */
    public List<TypeDesignationElement> getTypeDesignations(int typeDesignationsContainerIndex, String enclosingTag) {

        if(typeDesignationElementsByContainer == null){
            typeDesignationElementsByContainer = new ArrayList<>(Collections.nCopies(typeDesignationContainers.size(), null));
        }
        if(typeDesignationElementsByContainer.get(typeDesignationsContainerIndex) == null) {
            WebElement container = typeDesignationContainers.get(typeDesignationsContainerIndex);
            List<TypeDesignationElement> typeDesignationElements = new ArrayList<>();
            List<WebElement> childrenElements = container.findElements(By.cssSelector(":scope > " + enclosingTag)); // direct children
            for(WebElement we : childrenElements) {
                typeDesignationElements.add(new TypeDesignationElement(we));
            }
            typeDesignationElementsByContainer.add(typeDesignationsContainerIndex, typeDesignationElements);
        }
        return typeDesignationElementsByContainer.get(typeDesignationsContainerIndex);
    }
}