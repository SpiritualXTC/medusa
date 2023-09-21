import logging
import logging.config
import yaml
import coloredlogs

from medusa.utilities.resource import Resource


logger = logging.getLogger(__name__)


def init_logging(log_config_yml: Resource):
    coloredlogs.DEFAULT_LEVEL_STYLES = {
        "critical": {"bold": True, "color": "red"},
        "debug": {"color": "green"},
        "error": {"color": "red"},
        "info": {"color": "magenta"},
        "notice": {"color": "magenta"},
        "spam": {"color": "green", "faint": True},
        "success": {"bold": True, "color": "green"},
        "verbose": {"color": "blue"},
        "warning": {"color": "yellow"},
    }

    coloredlogs.DEFAULT_FIELD_STYLES = {
        "asctime": {"color": "yellow"},
        "hostname": {"color": "magenta"},
        "levelname": {"bold": True, "color": "black"},
        "name": {"color": "blue"},
        "programname": {"color": "cyan"},
        "username": {"color": "yellow"},
    }

    cfg = yaml.safe_load(log_config_yml.read())
    logging.config.dictConfig(cfg)

    logger.info("Initialised logging")
