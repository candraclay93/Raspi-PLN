import logging
from cysystemd.journal import JournaldLogHandler

Log = logging.getLogger(__name__)
journald_handler = JournaldLogHandler()
journald_handler.setFormatter(logging.Formatter(
    '[%(levelname)s] %(message)s'
))

Log.addHandler(journald_handler)
Log.setLevel(logging.DEBUG)
