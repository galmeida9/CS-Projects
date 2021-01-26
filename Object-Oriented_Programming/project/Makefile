classpath=.:sth-app/sth-app.jar:sth-core/sth-core.jar:/usr/share/java/po-uuilib.jar
java_flags=-Dimport=sth-app/people.import

all:
	(cd sth-core; make $(MFLAGS) all)
	(cd sth-app; make $(MFLAGS) all)

clean:
	(cd sth-core; make $(MFLAGS) clean)
	(cd sth-app; make $(MFLAGS) clean)

install:
	(cd sth-core; make $(MFLAGS) install)
	(cd sth-app; make $(MFLAGS) install)

run:
	java -cp $(classpath) $(java_flags) sth.app.App
