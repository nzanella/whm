
DROP VIEW AverageSellingPriceView;
DROP VIEW AveragePurchasingPriceView;
DROP VIEW OutgoingItemsView;
DROP VIEW IncomingItemsView;
DROP TABLE InventoryHistory;
DROP TABLE SnapshotDates;
DROP TABLE RefundSoldItems;
DROP TABLE ReceiveSoldItems;
DROP TABLE SendSoldItems;
DROP TABLE SellItems;
DROP TABLE Sales;
DROP TABLE Containers;
DROP TABLE ContainerCategories;
DROP TABLE TaxCategories;
DROP TABLE RefundPurchasedItems;
DROP TABLE ReturnPurchasedItems;
DROP TABLE ReceivePurchasedItems;
DROP TABLE PurchaseItems;
DROP TABLE Purchases;
DROP TABLE Items;
DROP TABLE Brands;
DROP TABLE Customers;
DROP TABLE Representatives;
DROP TABLE Suppliers;
DROP TABLE Banks;

CREATE TABLE Banks (
  bankid VARCHAR(64),
  bankname VARCHAR(64),
  bankstreet VARCHAR(64),
  bankcity VARCHAR(64),
  bankpostalcode VARCHAR(64),
  bankprovince VARCHAR(64),
  bankcountry VARCHAR(64),
  bankphonenumber VARCHAR(64),
  bankfaxnumber VARCHAR(64),
  bankemail VARCHAR(64),
  bankwebsite VARCHAR(64),
  bankswiftcode VARCHAR(64),
  banktransitnumber VARCHAR(64),
  bankother VARCHAR(255),
  PRIMARY KEY (bankid)
);

CREATE TABLE Suppliers (
  supid VARCHAR(64),
  supname VARCHAR(64),
  supstreet VARCHAR(64),
  supcity VARCHAR(64),
  suppostalcode VARCHAR(64),
  supprovince VARCHAR(64),
  supcountry VARCHAR(64),
  supphonenumber VARCHAR(64),
  supfaxnumber VARCHAR(64),
  supemail VARCHAR(64),
  supwebsite VARCHAR(64),
  supregistrationnumber VARCHAR(64),
  suptaxcode VARCHAR(64),
  supbankaccountnumber VARCHAR(64),
  bankid VARCHAR(64),
  supother VARCHAR(255),
  PRIMARY KEY (supid),
  FOREIGN KEY (bankid) REFERENCES Banks
);

CREATE TABLE Representatives (
  repid VARCHAR(64),
  repsaleslocation VARCHAR(64),
  repname VARCHAR(64),
  repstreet VARCHAR(64),
  repcity VARCHAR(64),
  reppostalcode VARCHAR(64),
  repprovince VARCHAR(64),
  repcountry VARCHAR(64),
  repworkphonenumber VARCHAR(64),
  rephomephonenumber VARCHAR(64),
  repcellphonenumber VARCHAR(64),
  repfaxnumber VARCHAR(64),
  repemail VARCHAR(64),
  repwebsite VARCHAR(64),
  repregistrationnumber VARCHAR(64),
  reptaxcode VARCHAR(64),
  repbankaccountnumber VARCHAR(64),
  bankid VARCHAR(64),
  repother VARCHAR(255),
  PRIMARY KEY (repid, repsaleslocation),
  FOREIGN KEY (bankid) REFERENCES Banks
);

CREATE TABLE Customers (
  cusid VARCHAR(64),
  cusname VARCHAR(64),
  cusstreet VARCHAR(64),
  cuscity VARCHAR(64),
  cuspostalcode VARCHAR(64),
  cusprovince VARCHAR(64),
  cuscountry VARCHAR(64),
  cusphonenumber VARCHAR(64),
  cusfaxnumber VARCHAR(64),
  cusemail VARCHAR(64),
  cuswebsite VARCHAR(64),
  cusregistrationnumber VARCHAR(64),
  custaxcode VARCHAR(64),
  cusbankaccountnumber VARCHAR(64),
  bankid VARCHAR(64),
  cusother VARCHAR(255),
  PRIMARY KEY (cusid),
  FOREIGN KEY (bankid) REFERENCES Banks
);

CREATE TABLE Brands (
  supid VARCHAR(64),
  brandname VARCHAR(64),
  PRIMARY KEY (supid, brandname),
  FOREIGN KEY (supid) REFERENCES Suppliers
    ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE Items (
  supid VARCHAR(64),
  brandname VARCHAR(64),
  itemcode VARCHAR(64),
  -- advertised unit purchasing price for this item
  advpurchprice DECIMAL DEFAULT 0.0,
  -- advertised unit selling price for this item
  advsellprice DECIMAL DEFAULT 0.0,
  -- number of warehouse items of given kind
  -- not stored in containers excluding damaged items
  numloose INTEGER DEFAULT 0,
  -- warehouse warehouse items of given kind
  -- which are damaged (none of which are stored in containers)
  numdamaged INTEGER DEFAULT 0,
  itemdesc VARCHAR(255),
  PRIMARY KEY (supid, brandname, itemcode),
  FOREIGN KEY (supid, brandname) REFERENCES Brands
  ON UPDATE CASCADE ON DELETE CASCADE,
  CHECK (numloose >= 0),
  CHECK (numdamaged >= 0)
);

CREATE TABLE Purchases (
  purchnum VARCHAR(64),
  purchdate DATE NOT NULL,
  purchdiscount DECIMAL,
  purchdesc VARCHAR(255),
  supid VARCHAR(64) NOT NULL,
  PRIMARY KEY (purchnum),
  FOREIGN KEY (supid) REFERENCES Suppliers
    ON UPDATE CASCADE
    -- cascading on deletes explicitly forbidden
);

CREATE TABLE PurchaseItems (
  purchnum VARCHAR(64),
  supid VARCHAR(64),
  brandname VARCHAR(64),
  itemcode VARCHAR(64),
  purchunitprice DECIMAL NOT NULL,
  numpurchased INTEGER NOT NULL,
  numpurchsatisfied INTEGER DEFAULT 0,
  PRIMARY KEY (purchnum, supid, brandname, itemcode),
  FOREIGN KEY (purchnum) REFERENCES Purchases
    ON UPDATE CASCADE,
    -- cascading on deletes enforced in application whenever appropriate
  FOREIGN KEY (supid, brandname, itemcode) REFERENCES Items
    ON UPDATE CASCADE,
    -- cascading on deletes explicitly forbidden
  CHECK (numpurchased > 0),
  CHECK (numpurchsatisfied >= 0)
);

CREATE TABLE ReceivePurchasedItems (
  purchrecdate DATE,
  purchnum VARCHAR(64),
  supid VARCHAR(64),
  brandname VARCHAR(64),
  itemcode VARCHAR(64),
  purchnumrec INTEGER,
  PRIMARY KEY (purchrecdate, purchnum, supid, brandname, itemcode),
  FOREIGN KEY (purchnum, supid, brandname, itemcode) REFERENCES PurchaseItems
    ON UPDATE CASCADE,
    -- cascading on deletes enforced in application whenever appropriate
  CHECK (purchnumrec > 0)
);

CREATE TABLE ReturnPurchasedItems (
  purchretdate DATE,
  purchnum VARCHAR(64),
  supid VARCHAR(64),
  brandname VARCHAR(64),
  itemcode VARCHAR(64),
  purchnumret INTEGER,
  PRIMARY KEY (purchretdate, purchnum, supid, brandname, itemcode),
  FOREIGN KEY (purchnum, supid, brandname, itemcode) REFERENCES PurchaseItems
    ON UPDATE CASCADE,
    -- cascading on deletes enforced in application whenever appropriate
  CHECK (purchnumret > 0)
);

CREATE TABLE RefundPurchasedItems (
  purchrefdate DATE,
  purchnum VARCHAR(64),
  supid VARCHAR(64),
  brandname VARCHAR(64),
  itemcode VARCHAR(64),
  purchnumref INTEGER,
  PRIMARY KEY (purchrefdate, purchnum, supid, brandname, itemcode),
  FOREIGN KEY (purchnum, supid, brandname, itemcode) REFERENCES PurchaseItems
    ON UPDATE CASCADE,
    -- cascading on deletes enforced in application whenever appropriate
  CHECK (purchnumref > 0)
);

CREATE TABLE TaxCategories (
  taxcat VARCHAR(64),
  taxper DECIMAL DEFAULT 0.0,
  PRIMARY KEY (taxcat)
);

CREATE TABLE ContainerCategories (
  ccatid VARCHAR(64),
  supid VARCHAR(64),
  brandname VARCHAR(64),
  taxcat VARCHAR(64),
  ccatdesc VARCHAR(64),
  ccatlongdesc VARCHAR(255),
  PRIMARY KEY (ccatid),
  FOREIGN KEY (supid, brandname) REFERENCES Brands
    ON UPDATE CASCADE ON DELETE CASCADE,
  FOREIGN KEY (taxcat) REFERENCES TaxCategories
    ON UPDATE CASCADE
    -- cascading on deletes explicitly forbidden
);

CREATE TABLE Containers (
  ccatid VARCHAR(64),
  contnum INTEGER,
  supid VARCHAR(64),
  brandname VARCHAR(64),
  itemcode VARCHAR(64),
  numstored INTEGER DEFAULT 0,
  PRIMARY KEY (ccatid, contnum),
  -- ensure one kind of items is not stored associated with more than
  -- one container identifier. In practice many containers may have the
  -- same container identifier and optionally subidentifiers.
  UNIQUE (supid, brandname, itemcode),
  FOREIGN KEY (ccatid) REFERENCES ContainerCategories
    ON UPDATE CASCADE,
    -- cascading on deletes enforced in application whenever appropriate
  FOREIGN KEY (supid, brandname, itemcode) REFERENCES Items
    ON UPDATE CASCADE,
    -- cascading on deletes enforced in application whenever appropriate
  CHECK (numstored >= 0)
);

CREATE TABLE Sales (
  salenum INTEGER,
  saledate DATE NOT NULL,
  repid VARCHAR(64) NOT NULL,
  repsaleslocation VARCHAR(64) NOT NULL,
  cusid VARCHAR(64) NOT NULL,
  salediscount DECIMAL,
  saledesc VARCHAR(255),
  PRIMARY KEY (salenum),
  FOREIGN KEY (cusid) REFERENCES Customers
    ON UPDATE CASCADE,
    -- cascading on deletes explicitly forbidden
  FOREIGN KEY (repid, repsaleslocation) REFERENCES Representatives
    ON UPDATE CASCADE
    -- cascading on deletes explicitly forbidden
);

CREATE TABLE SellItems (
  salenum INTEGER,
  supid VARCHAR(64),
  brandname VARCHAR(64),
  itemcode VARCHAR(64),
  fromccatid VARCHAR(64),
  fromcontnum INTEGER,
  numsold INTEGER,
  numsoldsatisfied INTEGER DEFAULT 0, -- number of items statisfied as sent
  saleunitprice DECIMAL,
  PRIMARY KEY (salenum, supid, brandname, itemcode),
  FOREIGN KEY (salenum) REFERENCES Sales
    ON UPDATE CASCADE,
    -- cascading on deletes enforced in application whenever appropriate
  FOREIGN KEY (supid, brandname, itemcode) REFERENCES Items
    ON UPDATE CASCADE,
    -- cascading on deletes explicitly forbidden
  CHECK (numsold > 0),
  CHECK (numsoldsatisfied >= 0)
);

CREATE TABLE SendSoldItems (
  salesenddate DATE,
  salenum INTEGER,
  supid VARCHAR(64),
  brandname VARCHAR(64),
  itemcode VARCHAR(64),
  supidused VARCHAR(64) NOT NULL,
  brandnameused VARCHAR(64) NOT NULL,
  itemcodeused VARCHAR(64) NOT NULL,
  salenumsent INTEGER NOT NULL,
  saleinvnum INTEGER,
  PRIMARY KEY (salesenddate, salenum, supid, brandname, itemcode,
               supidused, brandnameused, itemcodeused),
  FOREIGN KEY (salenum, supid, brandname, itemcode) REFERENCES SellItems
    ON UPDATE CASCADE,
    -- cascading on deletes explicitly forbidden
  FOREIGN KEY (supidused, brandnameused, itemcodeused) REFERENCES Items
    (supid, brandname, itemcode)
    ON UPDATE CASCADE,
    -- cascading on deletes explicitly forbidden
  CHECK (salenumsent > 0)
);

CREATE TABLE ReceiveSoldItems (
  salerecdate DATE,
  salenum INTEGER,
  supid VARCHAR(64),
  brandname VARCHAR(64),
  itemcode VARCHAR(64),
  supidused VARCHAR(64) NOT NULL,
  brandnameused VARCHAR(64) NOT NULL,
  itemcodeused VARCHAR(64) NOT NULL,
  salenumrec INTEGER NOT NULL,
  PRIMARY KEY (salerecdate, salenum, supid, brandname, itemcode,
               supidused, brandnameused, itemcodeused),
  FOREIGN KEY (salenum, supid, brandname, itemcode) REFERENCES SellItems
    ON UPDATE CASCADE,
    -- cascading on deletes explicitly forbidden
  FOREIGN KEY (supidused, brandnameused, itemcodeused) REFERENCES Items
    (supid, brandname, itemcode)
    ON UPDATE CASCADE,
    -- cascading on deletes explicitly forbidden
  CHECK (salenumrec > 0)
);

CREATE TABLE RefundSoldItems (
  salerefdate DATE,
  salenum INTEGER,
  supid VARCHAR(64),
  brandname VARCHAR(64),
  itemcode VARCHAR(64),
  supidused VARCHAR(64) NOT NULL,
  brandnameused VARCHAR(64) NOT NULL,
  itemcodeused VARCHAR(64) NOT NULL,
  salenumref INTEGER NOT NULL,
  PRIMARY KEY (salerefdate, salenum, supid, brandname, itemcode,
               supidused, brandnameused, itemcodeused),
  FOREIGN KEY (salenum, supid, brandname, itemcode) REFERENCES SellItems
    ON UPDATE CASCADE,
    -- cascading on deletes explicitly forbidden
  FOREIGN KEY (supidused, brandnameused, itemcodeused) REFERENCES Items
    (supid, brandname, itemcode)
    ON UPDATE CASCADE,
    -- cascading on deletes explicitly forbidden
  CHECK (salenumref > 0)
);

CREATE TABLE SnapshotDates (
  snapshotdate DATE,
  PRIMARY KEY (snapshotdate)
);

CREATE TABLE InventoryHistory (
  snapshotdate DATE,
  taxcat VARCHAR(64),
  taxper DECIMAL,
  ccatid VARCHAR(64),
  contnum INTEGER,
  supid VARCHAR(64),
  brandname VARCHAR(64),
  itemcode vARCHAR(64),
  advpurchprice DECIMAL,
  avpurchprice DECIMAL,
  advsellprice DECIMAL,
  avsellprice DECIMAL,
  numstored INTEGER,
  numloose INTEGER,
  numdamaged INTEGER,
  numincoming INTEGER,
  numoutgoing INTEGER,
  FOREIGN KEY (snapshotdate) REFERENCES SnapshotDates
     ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE VIEW IncomingItemsView AS
SELECT supid, brandname, itemcode,
       SUM(numpurchased) - SUM(numpurchsatisfied) AS numincoming
FROM Items NATURAL JOIN PurchaseItems
WHERE numpurchsatisfied < numpurchased
GROUP BY supid, brandname, itemcode;

CREATE VIEW OutgoingItemsView AS
SELECT supid, brandname, itemcode,
       SUM(numsold) - SUM(numsoldsatisfied) AS numoutgoing
FROM Items NATURAL JOIN SellItems
WHERE numsoldsatisfied < numsold
GROUP BY supid, brandname, itemcode;

CREATE VIEW AveragePurchasingPriceView AS
SELECT supid, brandname, itemcode,
       SUM(numpurchased * purchunitprice) / SUM(numpurchased) AS avpurchprice
FROM PurchaseItems
GROUP BY supid, brandname, itemcode;

CREATE VIEW AverageSellingPriceView AS
SELECT supid, brandname, itemcode,
       SUM(numsold * saleunitprice) / SUM(numsold) AS avsellprice
FROM SellItems
GROUP BY supid, brandname, itemcode;
