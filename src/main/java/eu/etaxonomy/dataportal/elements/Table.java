/**
* Copyright (C) 2021 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

/**
 * @author a.kohlbecker
 * @since Mar 18, 2021
 */
public class Table extends BaseElement {


    public Table(WebElement element) {
        super(element);
    }

    public static Table from(WebElement element) {
        if(element != null) {
            return new Table(element);
        }
        return null;
    }

    public BaseElement getHeaderCell(int cellId, int rowId) {
        return BaseElement.from(getElement().findElement(By.xpath("./thead[1]/tr[" + (rowId + 1) + "]/th[" + (cellId + 1)+ "]")));
    }

    public String getHeaderCellText(int cellId, int rowId) {
        BaseElement cell = getHeaderCell(cellId, rowId);
        if(cell!= null) {
            return cell.getText();
        }
        return null;
    }

    public BaseElement getBodyCell(int cellId, int rowId) {
        return BaseElement.from(getElement().findElement(By.xpath("./tbody[1]/tr[" + (rowId + 1) + "]/td[" + (cellId + 1) + "]")));
    }

    public String getBodyCellText(int cellId, int rowId) {
        BaseElement cell = getBodyCell(cellId, rowId);
        if(cell!= null) {
            return cell.getText();
        }
        return null;
    }

    public BaseElement getBodyCell(String cellText, int searchRowId, int returnRowId) {
        String xpathStr = "./tbody/tr/td[" + (searchRowId + 1) + "][text()='" + cellText + "']/following-sibling::td[" + (returnRowId - searchRowId) + "]";
        return BaseElement.from(getElement().findElement(By.xpath(xpathStr)));
    }

    public String getBodyCellText(String cellText, int rowId,  int returnRowId) {
        BaseElement cell = getBodyCell(cellText, rowId, returnRowId);
        if(cell!= null) {
            return cell.getText();
        }
        return null;
    }

}
