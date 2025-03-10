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
 */
public class TaxonSynonymyPage extends TaxonPage {

    protected static String drupalPagePathBase = "cdm_dataportal/taxon";

    @FindBy(id = "synonymy")
    @CacheLookup
    private WebElement synonymy;

    public TaxonSynonymyPage(WebDriver driver, DataPortalContext context, UUID taxonUuid) throws MalformedURLException {

        super(driver, context, taxonUuid, "synonymy");
    }

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
    public WebElement getNewHomotypicalGroupTypeDesignations() {
            WebElement typeDesignationElement = synonymy.findElement(By
                    .xpath("./div[contains(@class,'homotypic-synonymy-group')]/ul[contains(@class,'homotypicSynonyms')]/ul[contains(@class,'typeDesignations')]"));
            /*List<TypeDesignationElement> typeDesignations = new ArrayList<TypeDesignationElement>();
            for (WebElement el : typeDesignationElements) {
                typeDesignations.add(new TypeDesignationElement(el));
            }*/
            return typeDesignationElement;
        }
    public WebElement getNewHomotypicalGroupSynSecs() {
                WebElement synSecElement = synonymy.findElement(By
                        .xpath("./div[contains(@class,'homotypic-synonymy-group')]/ul[contains(@class,'homotypicSynonyms')]/ul[contains(@class,'synSecSources')]"));

                return synSecElement;
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
     *            the 1-based index of the heterotypical group
     */
    public WebElement getNewHeterotypicalGroupTypeDesignations(Integer heterotypicalGroupIndex) {
        WebElement typeDesignationElement = synonymy.findElement(By
                .xpath("./div[contains(@class,'heterotypic-synonymy-group')][" + heterotypicalGroupIndex
                        + "]/ul[contains(@class,'heterotypicSynonymyGroup')]/ul[contains(@class,'typeDesignations')]"));
        /*List<TypeDesignationElement> typeDesignations = new ArrayList<TypeDesignationElement>();
        for (WebElement el : typeDesignationElements) {
            typeDesignations.add(new TypeDesignationElement(el));
        }*/
        return typeDesignationElement;
    }

    public WebElement getNewHeterotypicalGroupSynSecs(Integer heterotypicalGroupIndex) {
        WebElement synSecElement = null;
        try{
            synSecElement = synonymy.findElement(By
                               .xpath("./div[contains(@class,'heterotypic-synonymy-group')][" + heterotypicalGroupIndex
                                        + "]/ul[contains(@class,'heterotypicSynonymyGroup')]/ul[contains(@class,'synSecSources')]"));

        } catch (Exception e) { /* IGNORE */}
        return synSecElement;
    }


    /**
     * @param heterotypicalGroupIndex
     * 				the 1-based index of the heterotypical group
     */
    public List<LinkElement> getHeterotypicalGroupFootNoteKeys(Integer heterotypicalGroupIndex) {
        // 1. try find the misapplied name footnote keys
        List<WebElement> fnkListElements = synonymy.findElements(
                By.xpath("./div[contains(@class,'heterotypic-synonymy-group')][" + heterotypicalGroupIndex + "]/ul[@class = 'heterotypicSynonymyGroup']/li/span/span/span/span[contains(@class, 'footnote-key')]/a")
         );
        // 2. try find the others
        if(fnkListElements.size() == 0){
            fnkListElements = synonymy.findElements(
                    By.xpath("./div[contains(@class,'heterotypic-synonymy-group')][" + heterotypicalGroupIndex + "]/ul[@class = 'heterotypicSynonymyGroup']/li/span/span[contains(@class, 'footnote-key')]/a")
             );
        }
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
//        WebElement typeDesignationElement = synonymy.findElement(By
//                .xpath("./div[contains(@class,'heterotypic-synonymy-group')][" + heterotypicalGroupIndex
//                        + "]/ul[contains(@class,'heterotypicSynonymyGroup')]/ul[contains(@class,'footnote')]"));

        return ElementUtils.baseElementsFromFootNoteListElements(fnListElements);
    }

    public WebElement getTaxonRelationships() {
        WebElement taxonRelationships = synonymy.findElement(
                By.xpath("./div[contains(@class,'taxon-relationships')]")
        );
        return taxonRelationships;
    }

    public WebElement getTaxonRelationships(Integer relatedTaxonIndex) {
        WebElement taxonRelationships = synonymy.findElement(
                By.xpath("./div[contains(@class,'taxon-relationships')]/ul[contains(@class,'taxonRelationships')]/li[" + relatedTaxonIndex + "]")
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
