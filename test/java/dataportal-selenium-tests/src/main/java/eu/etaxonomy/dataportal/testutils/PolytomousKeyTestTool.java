/**
 *
 */
package eu.etaxonomy.dataportal.testutils;

import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.junit.Assert;
import org.openqa.selenium.By;
import org.openqa.selenium.JavascriptExecutor;
import org.openqa.selenium.RenderedWebElement;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.ui.WebDriverWait;

import eu.etaxonomy.dataportal.selenium.JUnitWebDriverWait;
import eu.etaxonomy.dataportal.selenium.VisibilityOfElementLocated;
import eu.etaxonomy.dataportal.selenium.WebDriverFactory;

/**
 * @author a.kohlbecker
 *
 */
public class PolytomousKeyTestTool {

	public static final Logger logger = Logger.getLogger(PolytomousKeyTestTool.class);

	Map<Integer, KeyLineData> linesUnderTest; //  = new HashMap<Integer, KeyLineData>();
	List<WebElement> keyTableRows;
	private final WebDriver webDriver;
	private final JUnitWebDriverWait wait;

	public PolytomousKeyTestTool(List<WebElement> keyTableRows, Map<Integer, KeyLineData> linesUnderTest, WebDriver webDriver){
		this.linesUnderTest = linesUnderTest;
		this.keyTableRows = keyTableRows;
		this.webDriver = webDriver;
		wait = new JUnitWebDriverWait(webDriver, 15);
	}

	public void runTest(){
		for(Integer key : linesUnderTest.keySet()){
			testPolytomousKeyLine(keyTableRows.get(key), linesUnderTest.get(key));
		}
		webDriver.close();
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
		WebElement linkContainer = keyEntry.findElement(By.className(data.linkClass.name()));
		RenderedWebElement link = (RenderedWebElement)linkContainer.findElement(By.tagName("a"));
		Assert.assertEquals("link text", data.linkText, link.getText());
		logger.info("testing " +  data.linkClass.name() + " : " + link.getAttribute("href"));
		link.click();
		wait.until(new VisibilityOfElementLocated(By.id("container")));
		webDriver.navigate().back();
	}

}
