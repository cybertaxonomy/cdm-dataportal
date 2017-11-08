
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

import org.apache.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.CacheLookup;
import org.openqa.selenium.support.FindBy;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.ElementUtils;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.elements.TypeDesignationElement;

/**
 * TODO: subpages like /cdm_dataportal/taxon/{uuid}/images are not jet suported, implement means to handle page parts
 *
 * @author andreas
 * @since Jul 1, 2011
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
     */
    public TaxonSynonymyPage(WebDriver driver, DataPortalContext context, UUID taxonUuid) throws MalformedURLException {

        super(driver, context, taxonUuid.toString() + "/synonymy");

        this.taxonUuid = taxonUuid;
    }


    /**
     * @param driver
     * @param context
     */
    public TaxonSynonymyPage(WebDriver driver, DataPortalContext context) throws Exception {
        super(driver, context);
    }

    public String getAcceptedNameText() {
        return getAcceptedName().getText();
    }

    public WebElement getAcceptedName() {
        WebElement acceptedName = synonymy.findElement(
                By.xpath("./div[contains(@class,'accepted-name')]")
        );
        return acceptedName;
    }

    /**
     * TypeDesignation of the accepted name are found in the HomotypicalGroup block element
     * thus this method will delegate to {@link #getHomotypicalGroupTypeDesignations()}
     * @deprecated use {@link #getHomotypicalGroupTypeDesignations()} instead
     */
    @Deprecated
    public List<TypeDesignationElement> getAcceptedNameTypeDesignations() {
        return getHomotypicalGroupTypeDesignations();
    }

    /**
     * @return the getHomotypicalGroupFootNoteKeys()
     */
    public List<LinkElement> getAcceptedNameFootNoteKeys() {
        List<WebElement> fnkListElements = synonymy.findElements(
                By.xpath("./div[contains(@class,'accepted-name')]/span[contains(@class, 'footnote-key')]/a")
        );
        return ElementUtils.linkElementsFromFootNoteKeyListElements(fnkListElements);
    }

    /**
     * Footnotes of the accepted name are found in the HomotypicalGroup block element
     * thus this method will delegate to {@link #getHomotypicalGroupFootNotes()}
     *
     */
    @Deprecated
    public List<BaseElement> getAcceptedNameFootNotes() {
//        List<WebElement> fnListElements = synonymy.findElements(
//                By.xpath("./div[contains(@class,'accepted-name')]/following-sibling::ul[contains(@class, 'footnotes')]/li[contains(@class, 'footnotes')]/span[contains(@class, 'footnote')]")
//        );
//        return ElementUtils.baseElementsFromFootNoteListElements(fnListElements);
        return getHomotypicalGroupFootNotes();
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
     * @param synonymIndex the 1-based index of the synonym in the group
     */
    public WebElement getHomotypicalGroupSynonym(Integer synonymIndex) {
        WebElement synonym = synonymy.findElement(
                By.xpath("./div[contains(@class,'homotypic-synonymy-group')]/ul[contains(@class,'homotypicSynonyms')]/li[" + synonymIndex + "]")
        );
        return synonym;
    }

    public List<TypeDesignationElement> getHomotypicalGroupTypeDesignations() {
        List<WebElement> typeDesignationElements = synonymy.findElements(By
                .xpath("./div[contains(@class,'homotypic-synonymy-group')]/ul[contains(@class,'homotypicSynonyms')]/ul[contains(@class,'typeDesignations')]/li"));
        List<TypeDesignationElement> typeDesignations = new ArrayList<TypeDesignationElement>();
        for (WebElement el : typeDesignationElements) {
            typeDesignations.add(new TypeDesignationElement(el));
        }
        return typeDesignations;
    }

    public List<LinkElement> getHomotypicalGroupFootNoteKeys() {
        List<WebElement> fnkListElements = synonymy.findElements(
                By.xpath("./div[contains(@class,'homotypic-synonymy-group')]/ul[contains(@class,'homotypicSynonyms')]/*/span[contains(@class, 'footnote-key')]/a")
        );
        return ElementUtils.linkElementsFromFootNoteKeyListElements(fnkListElements);
    }

    public List<BaseElement> getHomotypicalGroupFootNotes() {
        List<WebElement> fnListElements = synonymy.findElements(
                By.xpath("./div[contains(@class,'homotypic-synonymy-group')]/ul[contains(@class,'footnotes')]/li[contains(@class, 'footnotes')]/span[contains(@class, 'footnote')]")
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
     */
    public WebElement getHeterotypicalGroupSynonym(Integer heterotypicalGroupIndex, Integer synonymIndex) {
        WebElement synonym = synonymy.findElement(By.xpath("./div[contains(@class,'heterotypic-synonymy-group')][" + heterotypicalGroupIndex + "]/ul[contains(@class,'heterotypicSynonymyGroup')]/li[" + synonymIndex + "]"));
        return synonym;
    }

    /**
     * @param heterotypicalGroupIndex
     *            the 1-based index of the heterotypical group
     */
    public List<TypeDesignationElement> getHeterotypicalGroupTypeDesignations(Integer heterotypicalGroupIndex) {
        List<WebElement> typeDesignationElements = synonymy.findElements(By
                .xpath("./div[contains(@class,'heterotypic-synonymy-group')][" + heterotypicalGroupIndex
                        + "]/ul[contains(@class,'heterotypicSynonymyGroup')]/ul[contains(@class,'typeDesignations')]/li"));
        List<TypeDesignationElement> typeDesignations = new ArrayList<TypeDesignationElement>();
        for (WebElement el : typeDesignationElements) {
            typeDesignations.add(new TypeDesignationElement(el));
        }
        return typeDesignations;
    }

    /**
     * @param heterotypicalGroupIndex
     * 				the 1-based index of the heterotypical group
     */
    public List<LinkElement> getHeterotypicalGroupFootNoteKeys(Integer heterotypicalGroupIndex) {
        List<WebElement> fnkListElements = synonymy.findElements(
                By.xpath("./div[contains(@class,'heterotypic-synonymy-group')][" + heterotypicalGroupIndex + "]/ul/*/*/span[contains(@class, 'footnote-key')]/a")
         );
        return ElementUtils.linkElementsFromFootNoteKeyListElements(fnkListElements);
    }

    /**
     * @param heterotypicalGroupIndex
     * 				the 1-based index of the heterotypical group
     */
    public List<BaseElement> getHeterotypicalGroupFootNotes(Integer heterotypicalGroupIndex) {
        List<WebElement> fnListElements = synonymy.findElements(
                By.xpath("./div[contains(@class,'heterotypic-synonymy-group')][" + heterotypicalGroupIndex + "]/ul/li[contains(@class, 'footnotes')]/span[contains(@class, 'footnote')]")
        );
        return ElementUtils.baseElementsFromFootNoteListElements(fnListElements);
    }

    public WebElement getTaxonRelationships() {
        WebElement taxonRelationships = synonymy.findElement(
                By.xpath("./div[contains(@class,'taxon-relationships')]")
        );
        return taxonRelationships;
    }

    public WebElement getMisappliedName(Integer misappliedNameIndex) {
        WebElement misappliedName = getTaxonRelationships().findElement(
                By.xpath("./ul[contains(@class,'misapplied')]/li[" + misappliedNameIndex + "]")
        );
        return misappliedName;
    }

}
