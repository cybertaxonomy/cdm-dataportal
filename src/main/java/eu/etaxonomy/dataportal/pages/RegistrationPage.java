/**
* Copyright (C) 2019 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.pages;

import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.util.Collections;
import java.util.UUID;

import org.apache.log4j.Logger;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.CacheLookup;
import org.openqa.selenium.support.FindBy;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.RegistrationItemFull;

/**
 * @author a.kohlbecker
 * @since Feb 4, 2019
 *
 */
public class RegistrationPage extends PortalPage {


    public static final Logger logger = Logger.getLogger(TaxonProfilePage.class);

    private UUID registrationUuid;

    protected static String drupalPagePathBase = "cdm_dataportal/registration";


    @FindBy(id = "registration")
    @CacheLookup
    private WebElement registrationElement;

    private RegistrationItemFull registrationItem;

    /**
     * @param driver
     * @param context
     * @throws Exception
     */
    public RegistrationPage(WebDriver driver, DataPortalContext context) throws Exception {
        super(driver, context);
    }


    public RegistrationPage(WebDriver driver, DataPortalContext context, String httpID) throws MalformedURLException, UnsupportedEncodingException {
        super(driver, context, null, Collections.singletonMap("identifier", httpID));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    protected String getDrupalPageBase() {
        return drupalPagePathBase;
    }

    /**
     * @return the registrationItem
     */
    public RegistrationItemFull getRegistrationItem() {

        if(registrationItem == null){
            registrationItem = new RegistrationItemFull(registrationElement);
        }
        return registrationItem;
    }







}
