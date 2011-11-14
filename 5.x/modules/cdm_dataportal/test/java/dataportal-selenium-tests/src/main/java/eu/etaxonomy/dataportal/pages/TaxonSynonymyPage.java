// $Id$

/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.pages;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import org.apache.log4j.Logger;
import org.junit.Assert;
import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.CacheLookup;
import org.openqa.selenium.support.FindBy;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.ElementUtils;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.elements.ImgElement;
import eu.etaxonomy.dataportal.elements.LinkElement;

/**
 * TODO: subpages like /cdm_dataportal/taxon/{uuid}/images are not jet suported, implement means to handle page parts
 *
 * @author andreas
 * @date Jul 1, 2011
 *
 */
public class TaxonSynonymyPage extends PortalPage {

    public static final Logger logger = Logger.getLogger(TaxonSynonymyPage.class);

    private UUID taxonUuid;

    protected static String drupalPagePathBase = "cdm_dataportal/taxon";

    /* (non-Javadoc)
     * @see eu.etaxonomy.dataportal.pages.PortalPage#getDrupalPageBase()
     */
    @Override
    protected String getDrupalPageBase() {
        return drupalPagePathBase;
    }

    @FindBy(id = "synonymy")
    @CacheLookup
    private WebElement synonymy;



    /**
     * @param driver
     * @param context
     * @param taxonUuid
     * @throws MalformedURLException
     */
    public TaxonSynonymyPage(WebDriver driver, DataPortalContext context, UUID taxonUuid) throws MalformedURLException {

        super(driver, context, taxonUuid.toString() + "/synonymy");

        this.taxonUuid = taxonUuid;
    }


    /**
     * @param driver
     * @param context
     * @throws Exception
     */
    public TaxonSynonymyPage(WebDriver driver, DataPortalContext context) throws Exception {
        super(driver, context);
    }


    /**
     * Returns the profile image of the taxon profile page. This image is
     * located at the top of the page. The Profile Image can be disabled in the
     * DataPortal settings.
     *
     * @return The Url of the profile image or null if the image is not visible.
     */
    public String getAcceptedName() {
        WebElement acceptedName = synonymy.findElement(
                By.xpath("./span[contains(@class,'accepted-name')]")
        );
        return acceptedName.getText();
    }

    public List<LinkElement> getAcceptedNameFootNoteKeys() {
        List<WebElement> fnkListElements = synonymy.findElements(
                By.xpath("./span[contains(@class,'accepted-name')]/following-sibling::span[contains(@class, 'footnote-key')]/a")
        );
        return ElementUtils.linkElementsFromFootNoteKeyListElements(fnkListElements);
    }

    /**
     * @param synonymIndex
     *            the 1-based position of the synonym in the list of homotypical
     *            synonyms
     * @return the full text line of the synonym including the prepending symbol
     *         and all information rendered after the name. All whitespace is
     *         normalized to the SPACE character.
     */
    public String getHomotypicalGroupSynonymName(Integer synonymIndex) {
        WebElement synonym = getHomotypicalGroupSynonym(synonymIndex);
        return synonym.getText().replaceAll("\\s", " ");
    }


    /**
     * @param synonymIndex
     * @return
     */
    public WebElement getHomotypicalGroupSynonym(Integer synonymIndex) {
        WebElement synonym = synonymy.findElement(
                By.xpath("./ul[contains(@class,'homotypicSynonyms')]/li[" + synonymIndex + "]")
        );
        return synonym;
    }

    public List<LinkElement> getHomotypicalGroupFootNoteKeys() {
        List<WebElement> fnkListElements = synonymy.findElements(
                By.xpath("./ul[contains(@class,'homotypicSynonyms')]/*/span[contains(@class, 'footnote-key')]/a")
        );
        return ElementUtils.linkElementsFromFootNoteKeyListElements(fnkListElements);
    }

    public List<BaseElement> getHomotypicalGroupFootNotes() {
        List<WebElement> fnListElements = synonymy.findElements(
                By.xpath("./ul[contains(@class,'homotypicSynonyms')]/li[contains(@class, 'footnotes')]/span[contains(@class, 'footnote')]")
        );
        return ElementUtils.baseElementsFromFootNoteListElements(fnListElements);

    }



    /**
     * @param heterotypicalGroupIndex
     *            the 1-based index of the heterotypical group
     * @param synonymIndex
     *            the 1-based position of the synonym in the list specified
     *            group of heterotypical synonyms
     * @return the full text line of the synonym including the prepending symbol
     *         and all information rendered after the name. All whitespace is
     *         normalized to the SPACE character.
     */
    public String getHeterotypicalGroupSynonymName(Integer heterotypicalGroupIndex, Integer synonymIndex) {
        WebElement synonym = getHeterotypicalGroupSynonym(heterotypicalGroupIndex, synonymIndex);
        return synonym.getText().replaceAll("\\s", " ");
    }


    /**
     * @param heterotypicalGroupIndex
     *            the 1-based index of the heterotypical group
     * @param synonymIndex
     *            the 1-based position of the synonym in the list specified
     *            group of heterotypical synonyms
     * @return
     */
    public WebElement getHeterotypicalGroupSynonym(Integer heterotypicalGroupIndex, Integer synonymIndex) {
        WebElement synonym = synonymy.findElement(By.xpath("./ul[contains(@class,'heterotypicSynonymyGroup')][" + heterotypicalGroupIndex + "]/li[" + synonymIndex + "]"));
        return synonym;
    }

    public List<LinkElement> getHeterotypicalGroupFootNoteKeys(Integer heterotypicalGroupIndex) {
        List<WebElement> fnkListElements = synonymy.findElements(
                By.xpath("./ul[contains(@class,'heterotypicSynonymyGroup')][" + heterotypicalGroupIndex + "]/*/span[contains(@class, 'footnote-key')]/a")
        );
        return ElementUtils.linkElementsFromFootNoteKeyListElements(fnkListElements);
    }

    public List<BaseElement> getHeterotypicalGroupFootNotes(Integer heterotypicalGroupIndex) {
        List<WebElement> fnListElements = synonymy.findElements(
                By.xpath("./ul[contains(@class,'heterotypicSynonymyGroup')][" + heterotypicalGroupIndex + "]/li[contains(@class, 'footnotes')]/span[contains(@class, 'footnotes')]")
        );
        return ElementUtils.baseElementsFromFootNoteListElements(fnListElements);
    }

}
