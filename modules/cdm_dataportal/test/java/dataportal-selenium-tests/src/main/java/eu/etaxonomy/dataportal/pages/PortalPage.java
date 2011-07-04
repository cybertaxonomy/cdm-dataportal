package eu.etaxonomy.dataportal.pages;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;
import org.junit.Assert;
import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.RenderedWebElement;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.CacheLookup;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.FindBys;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.LoadableComponent;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.TestConfiguration;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.selenium.JUnitWebDriverWait;

public abstract class  PortalPage {

	public static final Logger logger = Logger.getLogger(PortalPage.class);

	protected final static String DRUPAL_PAGE_QUERY_BASE = "?q=";

	protected WebDriver driver;

	protected final JUnitWebDriverWait wait;


	/**
	 * Implementations of this method will supply the raltive
	 * path to the Drupal page. This path will usally have the form
	 * <code>cdm_dataportal/{nodetype}</code>. For example the taxon pages all
	 * have the page base <code>cdm_dataportal/taxon</code>
	 * @return
	 */
	protected abstract String getDrupalPageBase();

	private String drupalPagePath;

	protected URL pageUrl;

	// ==== WebElements === //

	@FindBy(id="cdm_dataportal.node")
	protected RenderedWebElement portalContent;

	@FindBy(tagName="title")
	@CacheLookup
	protected RenderedWebElement title;

	@FindBy(className="node")
	protected RenderedWebElement node;


	@FindBys({@FindBy(id="tabs-wrapper"), @FindBy(className="primary")})
	@CacheLookup
	protected RenderedWebElement primaryTabs;


	public PortalPage(WebDriver driver, DataPortalContext context, String pagePathSuffix) throws MalformedURLException {

		this.driver = driver;

		this.wait = new JUnitWebDriverWait(driver, 25);

		this.drupalPagePath = getDrupalPageBase() + "/" + pagePathSuffix;

		this.pageUrl = new URL(context.getBaseUri().toString() + DRUPAL_PAGE_QUERY_BASE + drupalPagePath);

		// tell browser to navigate to the page
		driver.get(pageUrl.toString());

	    // This call sets the WebElement fields.
	    PageFactory.initElements(driver, this);


		logger.info("loading " + pageUrl);
	}

	public void get() {
		if(!driver.getCurrentUrl().equals(pageUrl.toString())){
			driver.get(pageUrl.toString());
			PageFactory.initElements(driver, this);
		}
	}

	public String getDrupalPagePath() {
		return drupalPagePath;
	}

	/**
	 * returns the string from the <code>title</code> tag.
	 * @return
	 */
	public String getTitle() {
		return title.getText();
	}

	public String getAuthorInformationText() {

		RenderedWebElement authorInformation = null;

		try {
			authorInformation  = (RenderedWebElement)node.findElement(By.className("submitted"));
		} catch (NoSuchElementException e) {
			// IGNORE //
		}


		if(authorInformation != null){
			return authorInformation.getText();
		} else {
			return null;
		}
	}

	public List<LinkElement> getPrimaryTabs(){
		List<LinkElement> tabs = new ArrayList<LinkElement>();
		List<WebElement> links = primaryTabs.findElements(By.tagName("a"));
		for(WebElement a : links) {
			RenderedWebElement renderedLink = (RenderedWebElement)a;
			if(renderedLink.isDisplayed()){
				tabs.add(new LinkElement(renderedLink));
			}
		}

		return tabs;
	}


	/**
	 * Returns the current URL string from the {@link WebDriver}
	 * @return
	 */
	public String getURL() {
		return driver.getCurrentUrl();
	}


	/**
	 * return the <code>scheme://domain:port</code> part of the initial url of this page.
	 * @return
	 */
	public String getInitialUrlBase() {
		return pageUrl.getProtocol() + "://" + pageUrl.getHost() + pageUrl.getPort();
	}

	public boolean equals(Object obj) {
		if (PortalPage.class.isAssignableFrom(obj.getClass())) {
			PortalPage page = (PortalPage) obj;
			return this.getURL().equals(page.getURL());

		} else {
			return false;
		}
	}


}
