package sth.app.main;

import java.io.FileNotFoundException;
import java.io.IOException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.Display;
import sth.SchoolManager;
import sth.app.exceptions.NoSuchPersonException;
import sth.exceptions.NoSuchPersonIdException;


/**
 * 4.1.1. Open existing document.
 */
public class DoOpen extends Command<SchoolManager> {

  private Input<String> _filename;
  
  /**
   * @param receiver
   */
  public DoOpen(SchoolManager receiver) {
    super(Label.OPEN, receiver);
    _filename = _form.addStringInput(Message.openFile());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
	public final void execute() throws NoSuchPersonException {
		Display d = new Display();
		_form.parse();
		try {
			_receiver.loadFile(_filename.value());
			for (String s: _receiver.sendNotifications())
				d = d.addLine(s);
			d.display();
		} catch (FileNotFoundException fnfe) {
    		_display.popup(Message.fileNotFound());
		} catch (ClassNotFoundException | IOException e) {
			e.printStackTrace();
		} catch (NoSuchPersonIdException e) {
			throw new NoSuchPersonException(e.getId());
		}
  	}

}
