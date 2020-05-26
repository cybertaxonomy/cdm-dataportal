
/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.pages;

import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import org.apache.commons.lang3.StringUtils;
import org.apache.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.TaxonNodeStatusElement;

/**
 * TODO: subpages like /cdm_dataportal/taxon/{uuid}/images are not yet supported, implement means to handle page parts
 *
 * @author andreas
 * @since Jul 1, 2011
 *
 */
public class TaxonPage extends PortalPage {

    public static final Logger logger = Logger.getLogger(TaxonProfilePage.class);

    protected static String drupalPagePathBase = "cdm_dataportal/taxon";


    @Override
    protected String getDrupalPageBase() {
        return drupalPagePathBase;
    }

    private UUID taxonUuid;

    private String subPage;


    public TaxonPage(WebDriver driver, DataPortalContext context, UUID taxonUuid) throws MalformedURLException {

        super(driver, context, taxonUuid.toString());

        this.taxonUuid = taxonUuid;
    }

    public TaxonPage(WebDriver driver, DataPortalContext context, UUID taxonUuid, String subPage) throws MalformedURLException {

        super(driver, context, taxonUuid.toString() + (!StringUtils.isEmpty(subPage) ? "/" + subPage : ""));

        this.taxonUuid = taxonUuid;
        this.subPage = subPage;
    }

    public TaxonPage(WebDriver driver, DataPortalContext context) throws Exception {
        super(driver, context);
    }


    public UUID getTaxonUuid() {
        return taxonUuid;
    }

    public String getSubPage() {
        return subPage;
    }

    public List<WebElement> getTaxonNodeStatusContainer() {
        List<WebElement> taxonNodeStatus = portalContent.findElements(By.className("taxon-node-status"));
        return taxonNodeStatus;
    }

    public List<TaxonNodeStatusElement> getTaxonNodeStatus() {
        List<TaxonNodeStatusElement> statusEls = new ArrayList<>();
        List<WebElement> taxonNodeStatus = portalContent.findElements(By.className("taxon-node-status"));
        for(WebElement el : taxonNodeStatus) {
            statusEls.add(new TaxonNodeStatusElement(el));
        }
        return statusEls;
    }

}
