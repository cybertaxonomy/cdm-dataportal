/**
* Copyright (C) 2021 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import org.openqa.selenium.WebElement;

/**
 * @author a.kohlbecker
 * @since Mar 18, 2021
 */
public class DetailsTable extends Table {

    boolean withLabelColumn = true;


    public DetailsTable(WebElement element) {
        super(element);
    }

    public static DetailsTable from(WebElement element) {
        if(element != null) {
            return new DetailsTable(element);
        }
        return null;
    }

    public String getHeaderText() {
        return getHeaderCellText(0, 0);
    }

    public BaseElement getDetailsValueCell(String labelText) {
        return getBodyCell(labelText, 0, 1);
    }

    public String getDetailsValueCellText(String labelText) {
        return getBodyCellText(labelText, 0, 1);
    }

    public static String tableClassAttrFrom(String headerLabel) {
        return headerLabel = "details-table-" + headerLabel
                .toLowerCase()
                .replaceAll("\\s", "_")
                .replaceAll("[^a-z0-9\\-_]", "");
    }



}
