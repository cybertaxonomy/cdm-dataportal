/**
 *
 */
package eu.etaxonomy.dataportal.testutils;

import java.util.List;
import java.util.Map;

import org.junit.Assert;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

/**
 * @author a.kohlbecker
 *
 */
public class PolytomousKeyTestTool {

	Map<Integer, KeyLineData> linesUnderTest; //  = new HashMap<Integer, KeyLineData>();
	List<WebElement> keyTableRows;

	public PolytomousKeyTestTool(List<WebElement> keyTableRows, Map<Integer, KeyLineData> linesUnderTest){
		this.linesUnderTest = linesUnderTest;
		this.keyTableRows = keyTableRows;
	}

	public void runTest(){
		for(Integer key : linesUnderTest.keySet()){
			testPolytomousKeyLine(keyTableRows.get(key), linesUnderTest.get(key));
		}
	}

	public static class KeyLineData{

		String nodeNumber = null;
		String edgeText = null;
		LinkClass linkClass = null;
		String linkText = null;

		public KeyLineData(String nodeNumber, String edgeText, LinkClass linkClass, String linkText) {
			this.nodeNumber = nodeNumber;
			this.edgeText = edgeText;
			this.linkClass = linkClass;
			this.linkText = linkText;
		}

	}

	public enum LinkClass {
		nodeLinkToNode,
		nodeLinkToTaxon;
	}

	public void testPolytomousKeyLine(WebElement keyEntry, KeyLineData data) {
		Assert.assertEquals("node number", data.nodeNumber, keyEntry.findElement(By.className("nodeNumber")).getText());
		Assert.assertEquals("edge text", data.edgeText + data.linkText, keyEntry.findElement(By.className("edge")).getText());
		WebElement link = keyEntry.findElement(By.className(data.linkClass.name()));
		Assert.assertEquals("link text", data.linkText, link.findElement(By.tagName("a")).getText());
	}

}
